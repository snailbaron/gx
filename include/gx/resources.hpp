#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <span>
#include <utility>

namespace gx {

template <class Id, class Object>
class ObjectCache {
public:
    Id push(Object&& object)
    {
        auto id = _nextId++;
        return _objects.emplace(id, std::move(object));
    }

    template <class... Args>
    Id emplace(Args&&... args)
    {
        auto id = _nextId++;
        _objects.emplace(id, Object{std::forward<Args>(args)...});
    }

    void pop(Id id)
    {
        return _objects.erase(id);
    }

    Object& operator[](Id id)
    {
        return _objects.at(id);
    }

    const Object& operator[](Id id) const
    {
        return _objects.at(id);
    }

private:
    std::map<Id, Object> _objects;
    Id _nextId = 0;
};

class Id {
public:
    using Base = unsigned long;

    explicit Id(Base id) : _id(id) {}
    operator Base() const { return _id; }

private:
    Base _id = 0;
};

class TextureId : public Id {};
class SpriteId : public Id {};
class ObjectId : public Id {};

struct SpriteLayout {
    enum class Type {
        Horizontal,
        Vertical,
    };

    Type type = Type::Horizontal;
    int frameCount = 0;
};

SpriteLayout horizontalLayout(int frameCount);
SpriteLayout verticalLayout(int frameCount);

class Resources {
public:
    TextureId loadTexture(const std::filesystem::path& path);
    TextureId loadTexture(const std::span<const std::byte>& data);

    SpriteId loadSprite(
        const std::filesystem::path& path,
        int fps,
        const SpriteLayout& layout = horizontalLayout(1));

private:
    ObjectCache<TextureId, Texture> _textures;
    ObjectCache<SpriteId, Sprite> _sprites;
};

} // namespace gx
