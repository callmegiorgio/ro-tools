#ifndef ROTOOLS_FORMAT_IMAGE_HPP
#define ROTOOLS_FORMAT_IMAGE_HPP

#include <memory>
#include "Pal.hpp"
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

namespace format {

struct Image {
    /// Saves to memory buffer.
    static void saveAsBmp(Buffer& buf, int width, int height, int channels, const void* pixels);

    explicit Image() = default;
    explicit Image(Image&&) = default;
    explicit Image(const Image&) = delete;
    explicit Image(const Buffer& buf) { load(buf); }

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidResource on failure.
     */
    void load(const Buffer& buf);

    int width;
    int height;
    int channels;
    std::unique_ptr<uint8_t[]> pixels;
};

} // namespace format

#endif // ROTOOLS_FORMAT_IMAGE_HPP