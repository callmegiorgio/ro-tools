#ifndef RO_ACT_HPP
#define RO_ACT_HPP

#include <array>
#include <string_view>
#include <vector>
#include "Buffer.hpp"
#include "Color.hpp"

class Act final {
public:
    struct SpriteClip
    {
        enum ImageType { Palette = 0, Rgba = 1 };

        int x;                          // offset in frame (0 for center)
        int y;                          // offset in frame (0 for center)
        int width = 0;                  // (v2.5+)
        int height = 0;                 // (v2.5+)
        int rotation = 0;               // (v2.0+) rotation angle in degrees
        float scale_x = 1.0f;           // (v2.4+) scale of X axis
        float scale_y = 1.0f;           // (v2.4+) scale of Y axis
        Color color{ 255, 255, 255 };   // (v2.0+)
        int sprite_number;              // number of the image in the spr file (-1 for none)
        bool mirror;                    // whether to mirror image along the vertical axis or not
        ImageType image_type = Palette; // (v2.0+) palette image or rgba image
    };

    struct AttachPoint {
        int x;
        int y;
        int attr;
    };

    // A Motion represents a frame in an action.
    struct Motion {
        int range1[4];
        int range2[4];
        int event_id = -1;                      // (v2.0+) -1 for none
        std::vector<SpriteClip> sprite_clips;   // max 1000
        std::vector<AttachPoint> attach_points; // (v2.3+)
    };

    // An Action is a collection of delayed motions.
    struct Action {
        std::vector<Motion> motions;
        float delay = 4.f;
    };

    class Event {
    public:
        explicit Event(std::array<char, 40> name) : name_{ std::move(name) } {}

        // Event name (e.g. "atk") or path of sound file.
        std::string_view name() const { return name_.data(); }

    private:
        std::array<char, 40> name_;
    };

    /**
     * Construct from memory buffer.
     *
     * @throw InvalidFile on failure
     */
    explicit Act(Buffer& buf);
    explicit Act(Buffer&& buf) : Act(buf) {}

    int versionMajor() const noexcept { return (version_ >> 8) & 0xFF; }
    int versionMinor() const noexcept { return version_ & 0xFF; }

    const std::vector<Action>& actions() const noexcept { return actions_; }
    const std::vector<Event>& events() const noexcept { return events_; }

private:
    void parse(Buffer& buf);

    int version_;
    std::vector<Action> actions_;
    std::vector<Event> events_; // (v2.1+)
};

#endif // RO_ACT_HPP