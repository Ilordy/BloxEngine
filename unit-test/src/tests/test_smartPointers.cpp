#include "test_smartPointers.h"
#include "core/blx_defines.h"
#include "utils/MemHelper.h"
#include <cstring>

BLXEXTERN_C_START
#include "../test_manager.h"
#include "../test_should.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"
BLXEXTERN_C_END

#include "utils/SharedPtr.h"
#include "utils/WeakPtr.h"

enum class PersonGender : char
{
    MALE,
    FEMALE
};

class Person
{
  private:
    const char* name;
    PersonGender gender;
    char age;

  public:
    Person(const char* name, PersonGender gender, char age) : gender(gender), age(age)
    {
        this->name = (char*)blxAllocate(strlen(name), BLXMEMORY_TAG_STRING);
        strcpy((char*)this->name, name);
    }

    ~Person()
    {
        blxFree((void*)name, BLXMEMORY_TAG_STRING);
    }

    const char* Name() const
    {
        return name;
    }

    const PersonGender Gender() const
    {
        return gender;
    }

    const char Age() const
    {
        return age;
    }
};

blxBool testSmartPointers_DoTests()
{
    BlxStl::SharedPtr<Person>* johnPtr = BlxMem::New<BlxStl::SharedPtr<Person>>(
        BLXMEMORY_TAG_SMARTPTR,
        BLXMEMORY_TAG_APPLICATION,
        "John Doe",
        PersonGender::MALE,
        30);

    BlxStl::WeakPtr<Person>* johnWeakPtr =
        BlxMem::New<BlxStl::WeakPtr<Person>>(BLXMEMORY_TAG_SMARTPTR, johnPtr);

    testShould_BeTrue(johnWeakPtr->IsValid());
    testShould_BeTrue(johnPtr->ReferenceCount() == 1);
    testShould_BeTrue(johnPtr->ObserverCount() == 1);

    testShould_BeTrue(johnWeakPtr->Get()->Age() == 30);
    testShould_BeTrue(johnWeakPtr->Get()->Gender() == PersonGender::MALE);
    testShould_BeTrue(strcmp(johnWeakPtr->Get()->Name(), "John Doe") == 0);

    testShould_BeTrue(johnPtr->Get()->Age() == 30);
    testShould_BeTrue(johnPtr->Get()->Gender() == PersonGender::MALE);
    testShould_BeTrue(strcmp(johnPtr->Get()->Name(), "John Doe") == 0);

    BlxStl::SharedPtr<Person>* johnPtr2 =
        BlxMem::New<BlxStl::SharedPtr<Person>>(BLXMEMORY_TAG_SMARTPTR, *johnPtr);

    testShould_BeTrue(johnPtr->ReferenceCount() == 2);
    testShould_BeTrue(johnPtr->ObserverCount() == 1);

    testShould_BeTrue(johnPtr2->ReferenceCount() == 2);
    testShould_BeTrue(johnPtr2->ObserverCount() == 1);

    testShould_BeTrue(johnPtr2->Get()->Age() == 30);
    testShould_BeTrue(strcmp(johnPtr2->Get()->Name(), "John Doe") == 0);
    testShould_BeTrue(johnPtr2->Get()->Gender() == PersonGender::MALE);

    BlxMem::DestructAndFree(johnPtr, BLXMEMORY_TAG_SMARTPTR);
    testShould_BeTrue(johnWeakPtr->IsValid());
    testShould_BeTrue(johnPtr2->ReferenceCount() == 1);
    BlxMem::DestructAndFree(johnPtr2, BLXMEMORY_TAG_SMARTPTR);

    testShould_BeFalse(johnWeakPtr->IsValid());
    BlxMem::DestructAndFree(johnWeakPtr, BLXMEMORY_TAG_SMARTPTR);

    return BLX_TRUE;
}

void testSmartPointers_RegisterTests()
{
    testManager_AddTest(testSmartPointers_DoTests);
}