// powerset.h
// powerset.h

#ifndef _POWERSET_H_INCLUDED_
#define _POWERSET_H_INCLUDED_

#include <vector>
#include "iterator_exceptions.h"
#include "set_adapter.h"

class powerset {
    public:
        class iterator : public std::iterator<std::forward_iterator_tag, std::vector<bool>> {
            public:
                iterator() : ref_set(nullptr), at_end(false)
                {
                }

                iterator(const iterator& other) : ref_set(other.ref_set)
                {
                }

                iterator(powerset* p, bool end) : ref_set(p), at_end(end)
                {
                }

                iterator& operator=(const iterator& other)
                {
                    if (this != &other) {
                        ref_set = other.ref_set;
                        at_end = other.at_end;
                    }
                    return *this;
                }

                ~iterator()
                {
                }

                const powerset& operator*() const
                {
                    if (ref_set == nullptr) {
                        throw iterator_not_dereferenceable_exception();
                    }
                    return *ref_set;
                }

                iterator operator++()
                {
                    if (ref_set != nullptr) {
                        ref_set->next();
                        at_end = ref_set->done();
                    }
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator it(*this); operator++(); return it;
                }

            private:

                powerset* ref_set;
                bool at_end;

            friend bool operator==(const iterator& a, const iterator& b)
            {
                return a.ref_set == b.ref_set && (*a).index() == (*b).index() && a.at_end == b.at_end;
            }

            friend bool operator!=(const iterator& a, const iterator& b)
            {
                return !(a == b);
            }

        };

        powerset(size_t N) : bitvec(N, false), listvec(), set_refid(0), last_subset(false), _done(false)
        {
            listvec.reserve(bitvec.size());
        }

        virtual ~powerset()
        {
        }

        iterator begin()
        {
            return iterator(this, false);
        }

        iterator end()
        {
            return iterator(this, true);
        }

        const std::vector<bool>& get_bitvec() const
        {
            return bitvec;
        }

        const std::vector<int>& get_listvec() const
        {
            return listvec;
        }

        virtual bool done() const
        {
            return _done;
        }

        size_t size() const
        {
            return bitvec.size();
        }

        size_t index() const
        {
            return set_refid;
        }

        virtual void next() = 0;

    protected:

        std::vector<bool> bitvec;
        std::vector<int> listvec;
        size_t set_refid;
        bool last_subset;
        bool _done;

};

class powerset_binary : public powerset {
    public:
        powerset_binary(size_t N) : powerset(N) {}
        ~powerset_binary() {}

        virtual void next()
        {
            next_inner();
            set_adapter::assign_listvec(listvec, bitvec);
        }

    private:
        virtual void next_inner()
        {
            if (last_subset) {    // one past the end
                _done = true;
                return;
            }
            size_t i = 0;
            while (bitvec[i]) {
                bitvec[i] = false;
                --set_refid;
                ++i;
            }
            bitvec[i] = true;
            ++set_refid;
            last_subset = set_refid == bitvec.size();    // have we generated the last subset?
        }

};

class powerset_graycode : public powerset {
    public:
        powerset_graycode(size_t N) : powerset(N) {}
        ~powerset_graycode() {}

        virtual void next()
        {
            next_inner();
            set_adapter::assign_listvec(listvec, bitvec);
        }

    private:
        virtual void next_inner()
        {
            if (last_subset) {    // one past the end
                _done = true;
                return;
            }
            size_t j = 0;
            if (set_refid % 2 != 0) {
                do {
                    ++j;
                } while (!bitvec[j-1]);
            }
            bitvec[j] = !bitvec[j];
            set_refid += 2 * bitvec[j] - 1;
            last_subset = (set_refid == 1 && bitvec[bitvec.size() - 1]);    // have we generated the last subset?
        }        
};

class powerset_lexicographic : public powerset {
    public:
        powerset_lexicographic(size_t N) : powerset(N), n(N), jump_over_supersets(false), started(false)
        {
            listvec.reserve(n);
        }

        powerset_lexicographic(size_t N, const std::vector<int>& v, bool jump = false) : powerset(N), n(N), jump_over_supersets(jump), started(false)
        {
            set_refid = v.size();
            listvec.resize(v.size());
            std::copy(v.begin(), v.end(), listvec.begin());
        }

        virtual void next()
        {
            next_inner();
            set_adapter::assign_bitvec(bitvec, listvec);
            _done = started && set_refid == 0;        // one past the end, in this case {}
        }

    private:
        void next_inner()
        {
            started = true;
            size_t is = 0;
            if (set_refid == 0) {
                if (jump_over_supersets) return;
                else {
                    ++set_refid;
                    listvec.push_back(-1);
                }
                listvec.back() = is;
                return;
            }
            if (listvec.back() != n-1) {
                is = listvec.back() + 1;
                if (!jump_over_supersets) {
                    ++set_refid;
                    listvec.push_back(-1);
                }
                listvec.back() = is;
                return;
            }
            --set_refid;
            listvec.pop_back();
            if (set_refid == 0) return;
            is = listvec.back() + 1;
            listvec.back() = is;
        }

    private:
        size_t n;
        bool jump_over_supersets;
        bool started;
};

#endif //_POWERSET_H_INCLUDED_
