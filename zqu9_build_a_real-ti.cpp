C++
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <SFML/Graphics.hpp>

struct DataPoint {
    float value;
    sf::CircleShape point;
};

class RealTimeDataViz {
public:
    RealTimeDataViz(int width, int height, int dataSize, int updateInterval) 
        : window(sf::VideoMode(width, height), "Real-Time Data Viz"), 
          dataSize_(dataSize), updateInterval_(updateInterval), 
          data_(dataSize, DataPoint()) {
        srand(time(0));
        for (auto& point : data_) {
            point.point.setRadius(2);
            point.point.setFillColor(sf::Color::White);
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            update();
            draw();

            std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval_));
        }
    }

private:
    void update() {
        for (auto& point : data_) {
            point.value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
        std::sort(data_.begin(), data_.end(), [](const DataPoint& a, const DataPoint& b) {
            return a.value < b.value;
        });
    }

    void draw() {
        window.clear();

        for (int i = 0; i < data_.size(); ++i) {
            data_[i].point.setPosition(i * 5, 200 - data_[i].value * 200);
            window.draw(data_[i].point);
        }

        window.display();
    }

    sf::RenderWindow window;
    int dataSize_;
    int updateInterval_;
    std::vector<DataPoint> data_;
};

int main() {
    RealTimeDataViz viz(800, 400, 200, 50);
    viz.run();
    return 0;
}