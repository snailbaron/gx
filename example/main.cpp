#include <gx.hpp>

#include <SDL.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <ostream>
#include <thread>

#include <iostream>

std::filesystem::path executableDirectory()
{
#ifdef __linux__
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#endif
}

struct Vector;

constexpr float length(const Vector& vector);
constexpr Vector normalize(const Vector& vector);
constexpr float distance(const Vector& lhs, const Vector& rhs);

struct Vector {
    constexpr Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr Vector& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    void resize(float newSize)
    {
        if (auto l = length(*this); l != 0) {
            *this *= newSize / l;
        }
    }

    float x = 0.f;
    float y = 0.f;
};

std::ostream& operator<<(std::ostream& output, const Vector& vector)
{
    return output << "(" << vector.x << ", " << vector.y << ")";
}

constexpr Vector operator+(Vector lhs, const Vector& rhs)
{
    lhs += rhs;
    return lhs;
}

constexpr Vector operator-(Vector lhs, const Vector& rhs)
{
    lhs -= rhs;
    return lhs;
}

constexpr Vector operator*(Vector lhs, float scalar)
{
    lhs *= scalar;
    return lhs;
}

constexpr Vector operator*(float scalar, Vector lhs)
{
    lhs *= scalar;
    return lhs;
}

constexpr Vector operator/(Vector lhs, float scalar)
{
    lhs /= scalar;
    return lhs;
}

constexpr float length(const Vector& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

constexpr Vector normalize(const Vector& vector)
{
    if (float l = length(vector); l != 0) {
        return vector / l;
    }
    return {.x = 0, .y = 0};
}

constexpr float distance(const Vector& lhs, const Vector& rhs)
{
    return length(lhs - rhs);
}

enum class ObjectType {
    Grass,
    Road,
    Tree,
    Stone,
};

struct KeyboardControl {
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
};

struct Object {
    ObjectType type = ObjectType::Grass;
    Vector position;
};

struct World {
    void initialize()
    {
        constexpr auto G = ObjectType::Grass;
        constexpr auto R = ObjectType::Road;
        constexpr auto T = ObjectType::Tree;
        constexpr auto S = ObjectType::Stone;
        using Row = std::array<ObjectType, 11>;

        constexpr auto data = std::array<Row, 11>{
            Row{T, T, T, T, T, R, T, T, T, T, T},
            Row{T, G, G, G, G, R, G, G, G, G, T},
            Row{T, G, G, S, G, R, G, G, G, G, T},
            Row{T, G, G, G, G, R, R, G, G, G, T},
            Row{T, G, G, G, G, G, R, G, G, G, T},
            Row{T, G, G, T, G, G, R, G, G, G, T},
            Row{T, G, G, G, G, G, R, R, R, G, T},
            Row{T, G, G, G, G, S, G, G, R, G, T},
            Row{T, G, G, G, G, G, G, G, R, G, T},
            Row{T, G, G, G, G, G, G, G, R, G, T},
            Row{T, T, T, T, T, T, T, T, R, T, T},
        };

        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                const auto& type = data.at(i).at(j);
                if (type == ObjectType::Tree || type == ObjectType::Stone) {
                    float x = (float)j - 5.f;
                    float y = 5.f - (float)i;
                    objects.push_back({
                        .type = data.at(i).at(j),
                        .position = {x, y}
                    });
                }
            }
        }
    }

    void update(float delta)
    {
        constexpr static float timeToSpeedup = 0.3f;
        constexpr static float timeToStop = 0.2;
        constexpr static float maxSpeed = 5.f;
        constexpr static float acceleration = maxSpeed / timeToSpeedup;
        constexpr static float deceleration = maxSpeed / timeToStop;

        auto controlAcceleration = controlVector();

        heroVelocity += controlAcceleration * (acceleration + deceleration) * delta;
        if (auto l = length(heroVelocity); l != 0) {
            float newLength = l - deceleration * delta;
            newLength = std::clamp(newLength, 0.f, maxSpeed);
            heroVelocity *= newLength / l;
        }

        heroPosition += heroVelocity * delta;

        for (const auto& object : objects) {
            if (float d = distance(heroPosition, object.position); d < 1) {
                auto pushVector = heroPosition - object.position;
                pushVector.resize(1 - d);
                heroPosition += pushVector;
            }
        }
    }

    Vector controlVector() const
    {
        auto controlVector = Vector{};
        if (control.moveLeft) {
            controlVector.x -= 1.f;
        }
        if (control.moveRight) {
            controlVector.x += 1.f;
        }
        if (control.moveUp) {
            controlVector.y += 1.f;
        }
        if (control.moveDown) {
            controlVector.y -= 1.f;
        }
        return normalize(controlVector);
    }

    bool processEvent(const SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            bool press = (e.type == SDL_KEYDOWN);
            if (e.key.keysym.sym == SDLK_a) {
                control.moveLeft = press;
                return true;
            }
            if (e.key.keysym.sym == SDLK_d) {
                control.moveRight = press;
                return true;
            }
            if (e.key.keysym.sym == SDLK_w) {
                control.moveUp = press;
                return true;
            }
            if (e.key.keysym.sym == SDLK_s) {
                control.moveDown = press;
                return true;
            }
        }
        return false;
    }

    Vector heroPosition;
    Vector heroVelocity;
    KeyboardControl control;
    std::vector<Object> objects;
};

struct Client {
    KeyboardControl keyboardControl;
};

struct Resources {
    struct Bitmaps {
        gx::Bitmap grass;
        gx::Bitmap tree;
        gx::Bitmap hero;
        gx::Bitmap stone;
    };

    struct Sprites {
        gx::Sprite grass;
        gx::Sprite tree;
        gx::Sprite hero;
        gx::Sprite stone;
    };

    Bitmaps bitmaps;
    Sprites sprites;
    gx::Cursor cursor;
};

Resources loadResources(const gx::Box& box)
{
    auto r = Resources{};

    const auto root = executableDirectory();

    r.bitmaps.grass = box.loadBitmap(root / "grass.png");
    r.sprites.grass = gx::createSimpleSprite(r.bitmaps.grass, 2, 3);

    r.bitmaps.tree = box.loadBitmap(root / "tree.png");
    r.sprites.tree = gx::createSimpleSprite(r.bitmaps.tree, 2, 3);

    r.bitmaps.hero = box.loadBitmap(root / "hero.png");
    r.sprites.hero = gx::createSimpleSprite(r.bitmaps.hero);

    r.bitmaps.stone = box.loadBitmap(root / "stone.png");
    r.sprites.stone = gx::createSimpleSprite(r.bitmaps.stone);

    r.cursor = gx::Box::loadCursor(root / "cursor.png", 2, 0);

    return r;
}

class Timer {
    using Clock = std::chrono::high_resolution_clock;

public:
    Timer(int fps)
        : _delta(1.0 / fps)
        , _frameDuration(std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>(_delta)))
        , _start(Clock::now())
    { }

    double delta() const
    {
        return _delta;
    }

    int operator()()
    {
        auto now = Clock::now();
        size_t frame = (now - _start) / _frameDuration;
        int framesPassed = static_cast<int>(frame - _lastFrame);
        _lastFrame = frame;
        return framesPassed;
    }

    void relax() const
    {
        std::this_thread::sleep_until(
            _start + _frameDuration * (_lastFrame + 1));
    }

private:
    double _delta = 0.0;
    Clock::duration _frameDuration;
    Clock::time_point _start;
    size_t _lastFrame = 0;
};

int main()
{
    auto world = World{};
    world.initialize();

    auto box = gx::Box{};
    auto r = loadResources(box);
    gx::Box::setCursor(r.cursor);

    auto* scene = box.createWidget<gx::Scene>();

    auto* hero = scene->spawn(r.sprites.hero, gx::WorldPoint{0, 0});
    scene->setupCamera(gx::WorldPoint{0, 0}, 16, 4);
    scene->cameraFollow(hero);

    for (const auto& object : world.objects) {
        switch (object.type) {
        case ObjectType::Stone:
            scene->spawn(r.sprites.stone, {object.position.x, object.position.y});
            break;
        case ObjectType::Tree:
            scene->spawn(r.sprites.tree, {object.position.x, object.position.y});
            break;
        default:
            break;
        }
    }

    static constexpr int fps = 60;
    auto timer = Timer{fps};

    for (;;) {
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            box.processEvent(e) || world.processEvent(e);
        }
        if (box.dead()) {
            break;
        }

        if (int framesPassed = timer(); framesPassed > 0) {
            // Update world with the same delta for perfect reproducibility
            for (int i = 0; i < framesPassed; i++) {
                world.update((float)timer.delta());
            }

            hero->position = {world.heroPosition.x, world.heroPosition.y};

            box.update(timer.delta() * framesPassed);
            box.present();
        }

        timer.relax();
    }
}
