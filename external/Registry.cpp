#include <Registry.hpp>

namespace m4x1m1l14n
{
    namespace Registry
    {
        // literally just this one key
        RegistryKey_ptr LocalMachine(new RegistryKey(HKEY_LOCAL_MACHINE));
    }
}
