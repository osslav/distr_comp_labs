#include "local_stat.hpp"

using namespace my_classes::stat;

LocalStat::LocalStat()
{
    for (auto& elem: most_freq_symbols_)
        elem = '\0';

    for (auto& elem: most_rare_symbols_)
        elem = '\0';
}

void LocalStat::updateMostStat_()
{
    std::multimap<uint32_t, char> stat_sorted_by_val;
    for (auto& elem: stat_)
        stat_sorted_by_val.insert(std::pair(elem.second, elem.first));

    auto r_iter = stat_sorted_by_val.rbegin();
    for (uint32_t i = 0; i < COUNT_MOST_FREQ && i < stat_sorted_by_val.size(); ++i)
        most_freq_symbols_[i] = (r_iter++)->second;
 
    auto iter = stat_sorted_by_val.begin();
    for (uint32_t i = 0; i < COUNT_MOST_RARE && i < stat_sorted_by_val.size(); ++i)
        most_rare_symbols_[i] = (iter++)->second;           

    most_stat_actual_for_count_ = count_all_symbol_;
}

void LocalStat::addSymbol(char new_s)
{
    auto elem = stat_.find(new_s);
    if (elem != stat_.end())
        ++(elem->second);
    else
        stat_.insert({new_s, 0});

    ++count_all_symbol_;
}

double LocalStat::getChance(char symb)
{
    auto elem = stat_.find(symb);
    if (elem != stat_.end())
        return static_cast<double>(elem->second) / count_all_symbol_; 
    else
        return 0;
}

std::array<char, COUNT_MOST_FREQ> LocalStat::getMostFreqSymb()
{
    if (count_all_symbol_ != most_stat_actual_for_count_)
        updateMostStat_();

    return most_freq_symbols_;
}

std::array<char, COUNT_MOST_RARE> LocalStat::getMostRareSymb()
{
    if (count_all_symbol_ != most_stat_actual_for_count_)
        updateMostStat_();

    return most_rare_symbols_;
}

void LocalStat::mergeNewStat(LocalStat& new_stat)
{
    stat_.merge(new_stat.stat_);

    for (auto elem: new_stat.stat_)
        stat_.find(elem.first)->second += elem.second;

    count_all_symbol_ += new_stat.count_all_symbol_;
    
    new_stat.stat_.clear();
    new_stat.count_all_symbol_ = 0;
}

