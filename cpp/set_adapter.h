// set_adapter.h

class set_adapter {
    public:
    static void assign_bitvec(std::vector<bool>& bitvec, const std::vector<int>& listvec)
    {
        bitvec.assign(bitvec.size(), false);
        for (auto it = listvec.begin(); it != listvec.end(); ++it) {
            bitvec[*it] = true;
        }
    }

    static std::vector<bool> to_bitvec(const std::vector<int>& listvec, size_t N)
    {
        std::vector<bool> bitvec(N, false);
        assign_bitvec(bitvec, listvec);
        return bitvec;
    }

    static void assign_listvec(std::vector<int>& listvec, const std::vector<bool>& bitvec)
    {
        listvec.clear();
        for (auto it = bitvec.begin(); it != bitvec.end(); ++it) {
            int idx = it - bitvec.begin();
            if (*it) listvec.push_back(idx);
        }	
    }

    static std::vector<int> to_listvec(const std::vector<bool>& bitvec)
    {
        std::vector<int> listvec;
        listvec.reserve(bitvec.size());
        assign_listvec(listvec, bitvec);
        return listvec;
    }    
};
