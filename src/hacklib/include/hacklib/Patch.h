#ifndef HACKLIB_PATCH_H
#define HACKLIB_PATCH_H

#include <cstdint>
#include <vector>
#include <cstddef>


namespace hl {


class Patch
{
public:
    ~Patch();

    // Applies a patch. Any previous patch done by the instance is reverted before.
    void apply(uintptr_t location, const char *patch, size_t size);

    template <typename T>
    void apply(uintptr_t location, T patch)
    {
        apply(location, (const char*)&patch, sizeof(patch));
    }

    void revert();

private:
    std::vector<unsigned char> m_backup;
    uintptr_t m_location = 0;
    size_t m_size = 0;

};

}

#endif
