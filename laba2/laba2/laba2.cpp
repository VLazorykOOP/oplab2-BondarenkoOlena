#include <iostream>
#include <thread>
#include <cmath>
#include <chrono>
#include <mutex>

const double V = 1.0;
const double R = 10.0;
const int sleep_duration = 100;
const int runtime_duration = 5000; // Час роботи потоків у мілісекундах

const double M_PI = 3.14159265358979323846;

std::mutex cout_mutex;

void worker_ant(double startX, double startY, double endX, double endY, int duration) {
    double currentX = startX;
    double currentY = startY;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed >= duration) break;

        while (currentX != endX || currentY != endY) {
            {
                std::lock_guard<std::mutex> guard(cout_mutex);
                std::cout << "Worker ant moving to [0,0]: (" << currentX << ", " << currentY << ")\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));

            if (currentX < endX) currentX += V;
            if (currentY < endY) currentY += V;
            if (currentX > endX) currentX -= V;
            if (currentY > endY) currentY -= V;

            if (abs(currentX - endX) < V) currentX = endX;
            if (abs(currentY - endY) < V) currentY = endY;

            current_time = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
            if (elapsed >= duration) break;
        }

        while (currentX != startX || currentY != startY) {
            {
                std::lock_guard<std::mutex> guard(cout_mutex);
                std::cout << "Worker ant returning: (" << currentX << ", " << currentY << ")\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));

            if (currentX < startX) currentX += V;
            if (currentY < startY) currentY += V;
            if (currentX > startX) currentX -= V;
            if (currentY > startY) currentY -= V;

            if (abs(currentX - startX) < V) currentX = startX;
            if (abs(currentY - startY) < V) currentY = startX;

            current_time = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
            if (elapsed >= duration) break;
        }
    }
}

void warrior_ant(int duration) {
    double angle = 0.0;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed >= duration) break;

        {
            std::lock_guard<std::mutex> guard(cout_mutex);
            std::cout << "Warrior ant at (" << R * cos(angle) << ", " << R * sin(angle) << ")\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));

        angle += V / R;
        if (angle >= 2 * M_PI) angle -= 2 * M_PI;
    }
}

int main() {
    int duration = runtime_duration;

    std::thread worker(worker_ant, 10.0, 10.0, 0.0, 0.0, duration);
    std::thread warrior(warrior_ant, duration);

    worker.join();
    warrior.join();

    return 0;
}
