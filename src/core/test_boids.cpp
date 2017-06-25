//
// Created by oliver on 11.05.17.
//

#include <vector>
#include <tbb/tbb.h>
#include "Vector3D.h"

using namespace std;
class Vector3D;
class Entity;

// todo: move stuff in another file

class Component{
public:
    virtual void update(Entity *pEntity) = 0;
};

class Entity{
public:
    Vector3D position;

    Entity() : position{0, 0, 0} {}

    vector<Component*> components;
    void update(){
        for(auto &value: this->components) {
            value->update(this);
        }
    }
};

class ConstantVelocityComponent : public Component{
private:
    Vector3D velocity;
public:
    ConstantVelocityComponent(const Vector3D &velocity) : velocity(velocity) {}

    virtual void update(Entity *pEntity){
        pEntity->position += velocity;
    }
};


//std::chrono::time_point<Time, fsec> game_start;
//float ms_since_gamestart = 0;
//std::chrono::time_point<Time, fsec> last_frame;
//float delta = 0;
//bool dirty = true;
//
//void update_time(){
//    if(dirty){
//        last_frame = Time::now();
//        game_start = Time::now();
//    }
//    dirty = false;
//
//    ms_since_gamestart = (Time::now() - game_start).count();
//    delta = (Time::now() - last_frame).count();
//    last_frame = Time::now();
//}
//
//typedef std::chrono::duration<double> dsec;
//
//void update(double t){
//    cout << t << endl;
//}
//
//void process_input(){}
//void render(double t){}
//
//void my_main(){
//    typedef std::chrono::high_resolution_clock Time;
//
//    double MS_PER_UPDATE = 20; // -> 50fps
//
//    auto start = Time.now();
//    auto previous = Time.now();
//    double lag = 0.0;
//    while(true){
//        /*  calc time */
//        auto current  = Time.now();
//        dsec elapsed = current - previous;
//        previous = current;
//
//        lag += elapsed.count();
//
//        process_input();
//
//        while(lag >= MS_PER_UPDATE){
//            update(elapsed.count());
//            lag -= MS_PER_UPDATE;
//        }
//
//        render(lag / MS_PER_UPDATE);
//    }
//
//}
//

/*
 *     typedef//
//typedef std::chrono::duration<double> dsec;
//
//void update(double t){
//    cout << t << endl;
//}
//
//void process_input(){}
//void render(double t){}
//
//void my_main(){
//    typedef std::chrono::high_resolution_clock Time;
//
//    double MS_PER_UPDATE = 20; // -> 50fps
//
//    auto start = Time.now();
//    auto previous = Time.now();
//    double lag = 0.0;
//    while(true){
//        /*  calc time */
//        auto current  = Time.now();
//        dsec elapsed = current - previous;
//        previous = current;
//
//        lag += elapsed.count();
//
//        process_input();
//
//        while(lag >= MS_PER_UPDATE){
//            update(elapsed.count());
//            lag -= MS_PER_UPDATE;
//        }
//
//        render(lag / MS_PER_UPDATE);
//    }
//
//}
//
//
// std::chrono::high_resolution_clock Time;
//    typedef std::chrono::milliseconds ms;
//
//
//    typedef std::chrono::duration<long,std::ratio<1,60>> sixtieths_of_a_sec;
//    constexpr auto kMaxDeltatime = sixtieths_of_a_sec{1};
//
//    auto mLastEndTime = Time::now();
//    // ms frameTime;
//
//    int cycles = 20;
//
//    while(--cycles > 0){
//
//        auto newEndTime = Time::now();
//        auto frameTime = newEndTime - mLastEndTime;
//        mLastEndTime = newEndTime;
//
//        while(frameTime > ms(0)){
//
//            typedef std::common_type_t <decltype(frameTime) , decltype(kMaxDeltatime)> common_duration;
//            auto deltaTime = std::min<common_duration >(frameTime, kMaxDeltatime);
//
//            frameTime -= deltaTime;
//            my_update(deltaTime);
//        }
//        // todo some break
//        std::this_thread::sleep_for(0.1s);
//    }
//


void test_boids(){

//    Entity e = Entity();
//    Vector3D v = {1, 0, 0};
//
//    ConstantVelocityComponent c = ConstantVelocityComponent(Vector3D(1, 0, 0));
//    e.components.push_back(&c);
//    // e.components.push_back(ConstantVelocityComponent(Vector3D(1, 0, 0)));
//
//    do{
//        update_time();
//        e.update();
//        cout << e.position << endl;
//        cout.flush();
//
//        cout << delta << endl;
//
//        while(std::chrono::duration_cast<ms>(Time::now() - last_frame).count() < 3){
//            cout << std::chrono::duration_cast<ms>(Time::now() - last_frame).count() << endl;
//        };
//
//    }while(ms_since_gamestart < 8*1e3);

}