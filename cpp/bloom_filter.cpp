// bloom_filter.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <cmath>
#include <vector>
#include <set>
#include <random>
#include <chrono>
#include <ios>

#include "openssl/md5.h"

void print_md5_sum(unsigned char* md) {
    int i;
    std::cout << "0x";
    for(int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)md[i];
    }
    std::cout << std::endl;
}

uint64_t hash_function(const std::string& msg, int fn, int m)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, &fn, sizeof(fn));
    MD5_Update(&ctx, msg.c_str(), msg.size());
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &ctx);
    return (*(uint64_t*)result) % m;
}

class bloom_filter {
public:
    bloom_filter(int m, int k) : m(m), k(k), filter(m, false)
    {
    }
    
    bool insert(const std::string& element)
    {
        for (int i = 0; i < k; ++i) {
            filter[hash_function(element, i, m)] = true;
        }
        return true;
    }
    
    bool contains(const std::string& element)
    {
        bool has_element = true;
        int i = 0;
        while (i < k && has_element) {
            has_element = filter[hash_function(element, i, m)];
            ++i;
        }
        return has_element;
    }
    
private:
    int m;
    int k;
    std::vector<bool> filter;
};


std::string random_string(int minlen, int maxlen, std::default_random_engine& rng)
{
    static const std::string symbols(
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");
        
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    int len = distribution(rng) * (maxlen - minlen) + minlen;
    std::string s(len, 0);
    for (int i = 0; i < len; ++i) {
        s[i] = symbols[distribution(rng) * symbols.length()];
    }
    return s;
}

int main()
{
    std::set<std::string> s;
    
    {   // read set
        std::ifstream infile("../data/enable.lexicon");
        std::string buf;
        while (std::getline(infile, buf)) {
            s.insert(buf);
        }
    }
    
    const double prob_false_positive = 1.0e-4;
    
    double factor = -log(prob_false_positive) / pow(log(2.0), 2.0);
    int m = int(factor * s.size() + 0.5);
    int k = int(double(m) / s.size() * log(2.0) + 0.5);
    
    std::cout << "p = " << std::scientific << prob_false_positive << std::fixed << std::endl;
    std::cout << "f = " << factor << std::endl;
    std::cout << "n = " << s.size() << std::endl;
    std::cout << "m = " << m << std::endl;
    std::cout << "k = " << k << std::endl;
    
    bloom_filter bf(m, k);
    std::default_random_engine generator;
    generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
    for (auto it = s.begin(); it != s.end(); ++it) {
        bf.insert(*it);
    }
    
    int false_pos = 0;
    int false_neg = 0;
    int N = 100000;
    for (int i = 0; i < N; ++i) {
        std::string x = random_string(4,10,generator);
        bool in_set = s.find(x) != s.end();
        bool in_bf = bf.contains(x);
        if (in_set && !in_bf) {
            ++false_neg;
        } else if (!in_set && in_bf) {
            ++false_pos;
        }
    }
    std::cout << "False Pos: " << false_pos << ", " << std::scientific << double(false_pos) / N << std::fixed << std::endl;
    std::cout << "False Neg: " << false_neg << ", " << std::scientific << double(false_neg) / N << std::fixed << std::endl;
    
}


