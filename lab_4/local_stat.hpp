#pragma once

#include <map>
#include <array>
#include <cstdint>
#include <mutex>

namespace my_classes::stat
{    
    const uint32_t COUNT_MOST_FREQ = 5;
    const uint32_t COUNT_MOST_RARE = 3;


    class LocalStat
    {
    private:
        std::map<char, uint32_t> stat_;
        uint32_t count_all_symbol_ = 0;

        std::array<char, COUNT_MOST_FREQ> most_freq_symbols_;
        std::array<char, COUNT_MOST_RARE> most_rare_symbols_;
        uint32_t most_stat_actual_for_count_ = 0;

        void updateMostStat_();
    public:
        LocalStat();
        virtual ~LocalStat() = default;

        virtual void addSymbol(char new_s);
        
        virtual double getChance(char symb);
        
        virtual std::array<char, COUNT_MOST_FREQ> getMostFreqSymb();
        virtual std::array<char, COUNT_MOST_RARE> getMostRareSymb();
       
        virtual void mergeNewStat(LocalStat& new_stat);
    };
}
