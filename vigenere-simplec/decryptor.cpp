
#include <iostream>
#include <stack>
#include <string>
#include "TinySHA1.hpp"
#include <thread>
#include <mutex>
#include <list>
#include <fstream>
#include <vector>

uint32_t startOfKeyHashHex = 0xd51ab000;
std::string startOfTheKey = "53cr3t-";
std::mutex mutex;
std::ofstream possibleHashes{"keys.txt", std::ios::binary};
std::list<std::string> listOfHashes;

uint32_t GetFirstDigest(const std::string &key)
{
	sha1::SHA1 sha;
	sha.processBytes(key.c_str(), key.size());
	uint32_t digest[5];
	sha.getDigest(digest);
	digest[0] &= ~(0xFFF);
	/* Set last 1,5 bytes to zero, cause our hash has only first 2,5*/
	return digest[0];
}

std::string GetSHA1(const std::string &key)
{
	sha1::SHA1 sha;
	sha.processBytes(key.c_str(), key.size());
	uint32_t digest[5];
	sha.getDigest(digest);
	char tmp[48];
	snprintf(tmp, 45, "%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3], digest[4]);
	return tmp;
}

void compute(uint64_t min, uint64_t max)
{
	for (uint64_t i = min; i < max; i++)
	{
		std::string key = startOfTheKey + std::to_string(i);

		if (const auto hash = GetFirstDigest(key); hash == startOfKeyHashHex)
		{
			mutex.lock();
			listOfHashes.emplace_back(GetSHA1(key));
			mutex.unlock();
		}
	}
}

int main()
{
	const auto cores = 8;
	auto index = 4294967296 / cores;
	auto chunk = index;

	std::thread threads[cores];

	uint64_t now = 0;
	for (int i = 0; i < cores; ++i, now += chunk)
		threads[i] = std::thread{[=]() {
			compute(now, now + chunk);
		}};

	for (auto &&thread : threads)
		thread.join();

	for (auto &&el : listOfHashes)
		possibleHashes << el << "\n";
}
