#pragma once

#include <map>
#include <array>
#include <cstdint>
#include <mutex>

namespace my_classes::stat
{    
    class GeneralStat: public LocalStat
    {
    private:
        mutable std::mutex mutex_stat_;

    public:
        virtual void addSymbol(char new_s)
        {
            std::lock_guard lock(mutex_stat_);
            LocalStat::addSymbol(new_s);
        }

        virtual double getChance(char symb)
        {
            std::lock_guard lock(mutex_stat_);
            return LocalStat::getChance(symb);
        }

        virtual std::array<char, COUNT_MOST_FREQ> getMostFreqSymb()
        {
            std::lock_guard lock(mutex_stat_);
            return LocalStat::getMostFreqSymb();
        }

        virtual std::array<char, COUNT_MOST_RARE> getMostRareSymb()
        {
            std::lock_guard lock(mutex_stat_);
            return LocalStat::getMostRareSymb();
        }

        virtual void mergeNewStat(LocalStat& new_stat)
        {
            std::lock_guard lock(mutex_stat_);
            LocalStat::mergeNewStat(new_stat);
        }
    };
}
