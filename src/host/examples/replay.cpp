#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdint>
#include <cmath>
#include <chrono>

#include <algebra/quaternion.hh>
#include <algebra/moebius.hh>
#include <geometry/hyperbolic.hh>
#include <geometry/hyperbolic/plane.hh>
#include <geometry/hyperbolic/horosphere.hh>
#include <view.hh>
#include <object.hh>

#include <opencl/search.hpp>
#include <sdl/viewer.hpp>
#include <sdl/controller.hpp>
#include <sdl/image.hpp>
#include <renderer.hpp>
#include <scenario.hpp>
#include <color.hpp>

#include "scene.hpp"


using duration = std::chrono::duration<double>;

class MyScenario : public PathScenario {
    public:
    std::vector<Object> get_objects(double t) const override {
        return std::vector<Object>();
    }
};

int main(int argc, const char *argv[]) {
    int platform_no = 0;
    int device_no = 0;
    try {
        if (argc >= 2) {
            platform_no = std::stoi(argv[1]);
            if (argc >= 3) {
                device_no = std::stoi(argv[2]);
            }
        }
    } catch(...) {
        std::cerr << "Invalid argument" << std::endl;
        return 1;
    }

    std::cout << "Using platform " << platform_no << ", device " << device_no << std::endl;
    cl_device_id device = cl::search_device(platform_no, device_no);

    int width = 1280, height = 720;
    Renderer renderer(device, width, height, Renderer::Config {
        .path_max_depth = 3,
        .path_max_diffuse_depth = 2,
        .blur = { .lens = true, .motion = true, .object_motion = false },
        .gamma = 2.2
    });
    renderer.store_objects(create_scene());

    Viewer viewer(width, height);
    Controller controller;

    std::vector<View> points {
        view_position(mo_new(
            c_new(0.0704422, 0.388156),
            c_new(3.25709, -0.644618),
            c_new(0.0280217, 0.0111143),
            c_new(0.542426, -2.73144)
        )),
        view_position(mo_new(
            c_new(0.0286821, 0.683827),
            c_new(2.8249, -1.7697),
            c_new(-0.0203855, 0.451223),
            c_new(2.02, -2.46116)
        )),
        view_position(mo_new(
            c_new(0.605603, -0.0125093),
            c_new(1.13499, -2.28722),
            c_new(0.296042, -0.0701192),
            c_new(1.96622, -1.20888)
        )),
        view_position(mo_new(
            c_new(0.155695, -0.546314),
            c_new(-1.72239, -0.323492),
            c_new(-0.0113209, -0.0551297),
            c_new(0.316326, 1.74335)
        )),
        view_position(mo_new(
            c_new(0.0748133, -0.111905),
            c_new(-5.96229, 0.107037),
            c_new(0.0470794, -0.0507573),
            c_new(1.09217, 5.74615)
        )),
    };
    
    MyScenario scenario;
    std::vector<SquareTransition> transitions {
        SquareTransition(8.0, points[0], points[1], 0.0, 1.0),
        SquareTransition(8.0, points[1], points[2], 0.0, 1.0),
        SquareTransition(8.0, points[2], points[3], 0.0, 1.0),
        SquareTransition(8.0, points[3], points[4], 0.0, 1.0)
    };
    
    for (const auto &t : transitions) {
        scenario.add_transition(std::make_unique<SquareTransition>(t));
    }

    //controller.view.lens_radius = 1e-1;

    int counter = 0;
    double time = 0.0;
    double frame_time = 0.04;
    duration time_counter;
    int sample_counter = 0;
    for(;;) {
        auto start = std::chrono::system_clock::now();

        //v.field_of_view = 0.8;
        renderer.set_view(
            scenario.get_view(time),
            scenario.get_view(time - frame_time)
        );
        sample_counter += renderer.render_for(frame_time, true);
        //sample_counter += renderer.render_n(100, true);
        time += frame_time;

        auto store = [&](uint8_t *data) {
            renderer.load_image(data);
        };
        viewer.display(store);

        //std::stringstream ss;
        //ss << std::setfill('0') << std::setw(5) << "output/" << counter << ".png";
        //sdl::save_image(ss.str(), width, height, store);

        if (!controller.handle() || time > scenario.duration()) {
            break;
        }
        counter += 1;

        time_counter += std::chrono::system_clock::now() - start;
        if (time_counter.count() > 1.0) {
            std::cout << "Elapsed: " << time << " / " << scenario.duration() << std::endl;
            std::cout << "Samples per second: " <<
                sample_counter/time_counter.count() << std::endl;
            time_counter = duration(0.0);
            sample_counter = 0;
        }
    }

    return 0;
}

