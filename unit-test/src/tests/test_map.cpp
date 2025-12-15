#include "test_map.h"
#include "BLX/utils/Map.h"
#include "core/blx_defines.h"
#include "core/blx_logger.h"

BLXEXTERN_C_START
#include "../test_manager.h"
#include "../test_should.h"
#include "BLX/core/blx_memory.h"
BLXEXTERN_C_END

#include <string.h>

class String
{
  private:
    char* buffer;
    uint64 bufferSize;

  public:
    String(const char* str)
    {
        bufferSize = strlen(str) + 1;
        buffer = (char*)blxAllocate(bufferSize, BLXMEMORY_TAG_STRING);
        strcpy(buffer, str);
    }

    // Copy constructor
    String(const String& other)
    {
        bufferSize = other.bufferSize;
        buffer = (char*)blxAllocate(bufferSize, BLXMEMORY_TAG_STRING);
        strcpy(buffer, other.buffer);
    }

    // Copy assignment operator
    String& operator=(const String& other)
    {
        if (this != &other)
        {
            blxFree(buffer, BLXMEMORY_TAG_STRING);
            bufferSize = other.bufferSize;
            buffer = (char*)blxAllocate(bufferSize, BLXMEMORY_TAG_STRING);
            strcpy(buffer, other.buffer);
        }
        return *this;
    }

    // Move constructor
    String(String&& other) noexcept
    {
        buffer = other.buffer;
        bufferSize = other.bufferSize;

        other.buffer = nullptr;
        other.bufferSize = 0;
    }

    // Move assignment operator
    String& operator=(String&& other) noexcept
    {
        if (this != &other)
        {
            blxFree(buffer, BLXMEMORY_TAG_STRING);
            buffer = other.buffer;
            bufferSize = other.bufferSize;

            other.buffer = nullptr;
            other.bufferSize = 0;
        }
        return *this;
    }

    ~String()
    {
        blxFree(buffer, BLXMEMORY_TAG_STRING);
    }

    bool operator==(const String& other) const
    {
        return strcmp(buffer, other.buffer) == 0;
    }

    const char& operator[](uint64 index) const
    {
        return buffer[index];
    }

    char& operator[](uint64 index)
    {
        return buffer[index];
    }

    const char* c_str() const
    {
        return buffer;
    }

    uint64 Length() const
    {
        return bufferSize - 1;
    }
};

namespace BlxStl
{
    template <>
    struct Hash<String>
    {
        uint64 operator()(const String& str)
        {
            uint64 hash = 0;
            for (uint64 i = 0; i < str.Length(); ++i)
            {
                hash = (hash * 31 + str[i]) % 1000000007;
            }
            return hash;
        }
    };
} // namespace BlxStl

blxBool testMap_DoTests()
{
    BlxStl::Map<String, int> map;
    
    map.Add("Jerry", 45);
    testShould_BeTrue(map.Contains("Jerry"));
    testShould_BeTrue(map["Jerry"] == 45);

    map["Jerry"] = 50;
    testShould_BeTrue(map["Jerry"] == 50);
    
    map.Add("Tom", 23);
    testShould_BeTrue(map.Contains("Tom"));
    testShould_BeTrue(map["Tom"] == 23);

    map.Remove("Tom");
    testShould_BeFalse(map.Contains("Tom"));

    map.Add("Tom", 25);
    testShould_BeTrue(map.Contains("Tom"));
    testShould_BeTrue(map["Tom"] == 25);

    String bob = "Bob";
    map.Add(bob, 34);

    testShould_BeTrue(map.Contains("Bob"));
    testShould_BeTrue(map.Contains(bob));
    testShould_BeTrue(map.Contains(&bob));

    testShould_BeTrue(map[bob] == 34);
    testShould_BeTrue(map["Bob"] == 34);

    map["Bob"] = 40;
    testShould_BeTrue(map[bob] == 40);
    testShould_BeTrue(map["Bob"] == 40);

    map.Remove(bob);
    testShould_BeFalse(map.Contains("Bob"));
    testShould_BeFalse(map.Contains(bob));
    testShould_BeFalse(map.Contains(&bob));

    map.Add(bob, 50);
    testShould_BeTrue(map.Contains("Bob"));
    testShould_BeTrue(*map.Find("Bob") == 50);
    testShould_BeTrue(map[bob] == 50);

    return BLX_TRUE;
}

void testMap_RegisterTests()
{
    testManager_AddTest(testMap_DoTests);
}