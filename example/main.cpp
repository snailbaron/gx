#include <gx.hpp>

#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <thread>

#include <iostream>

std::filesystem::path executableDirectory()
{
#ifdef __linux__
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#endif
}

int main()
{
    const auto root = executableDirectory();

    auto client = gx::Client{};

    auto treeSprite = client.loadSprite(
        root / "tree.png", 5, gx::horizontalLayout(2));

    static constexpr int fps = 60;
    static constexpr double frameDurationSeconds = 1.0 / fps;

    using Clock = std::chrono::high_resolution_clock;
    static constexpr auto frameDuration = std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>{frameDurationSeconds});

    const auto startTime = Clock::now();
    size_t lastFrameIndex = 0;
    for (;;) {
        bool done = false;
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            if (e.type == SDL_QUIT) {
                done = true;
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                done = true;
            }
        }
        if (done) {
            break;
        }

        auto currentTime = Clock::now();
        auto frameIndex = (currentTime - startTime) / frameDuration;
        unsigned framesPassed = frameIndex - lastFrameIndex;
        lastFrameIndex = frameIndex;

        if (framesPassed > 0) {
            client.update(framesPassed * frameDurationSeconds);
            client.present();
        }

        std::this_thread::sleep_until(
            startTime + frameDuration * (frameIndex + 1));
    }
}
