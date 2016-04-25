#include <experimental/optional>
#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <array>

template<typename T> void show_size() {
	std::experimental::optional<T> o;
	T t;
	printf("sizeof T: %lu optional<T>: %lu\n", sizeof(t), sizeof(o));
}

int main(int argc, const char *argv[]) {
	show_size<char>();
	show_size<int16_t>();
	show_size<int32_t>();
	show_size<int64_t>();
	show_size<std::string>();

	do {
		try {
			if (argc > 1) throw argv[1];
		} catch (const char* e) {
			std::cout << "error: " << e << std::endl;
			break;
		}
		std::cout << "last line in the loop" << std::endl;
	} while (0);

	std::cout << "got out of the loop" << std::endl;

	return 0;
}
