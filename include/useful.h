#include <memory.h>
#include <utility>



template <typename P, typename T>
std::vector<std::shared_ptr<T>> vectorOfSharedPointerStaticCast(const std::vector<std::shared_ptr<P>> & start_vc)
{
    std::vector<std::shared_ptr<T>> to_return;

    for (auto el : start_vc)    {
        to_return.push_back(std::pointer_static_cast<T>(el));
    }

    return to_return;

}