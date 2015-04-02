#include <functional>
#include <unordered_map>

void route(std::string url, std::function<void(const int, int)> func)
{ 
	std::unordered_map<std::string, std::function<void(const int, int)>&  > url_callback_map;
	url_callback_map.emplace(url, func);
}
