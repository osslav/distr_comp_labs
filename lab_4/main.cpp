#include "local_stat.hpp"
#include "general_stat.hpp"

#include <iostream>
#include <fstream>

#include <stdexcept>

#include <vector>
#include <string>

#include <thread>
#include <atomic>

using namespace my_classes;

uint32_t askCount(const char* question_msg)
{
    std::cout << question_msg;
    uint32_t count_msg = 0;
    std::cin >> count_msg;

    if (!std::cin)
    {
	    std::cin.clear();
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    throw std::runtime_error("Invalid input");
    }
    return count_msg;
}

void reader_func(const std::vector<std::string>& filenames, stat::GeneralStat& g_stat, std::atomic<uint32_t>& next_unread_file)
{
    stat::LocalStat l_stat;
            
    uint32_t current_index_file = next_unread_file.fetch_add(1);
    while(current_index_file < filenames.size())
    {
        std::ifstream file_stream(filenames[current_index_file]);
        char curr_symb = '\0';

        while(!(file_stream >> curr_symb).eof())
        {
            //std::cout << curr_symb;
            //file_stream >> curr_symb;
            l_stat.addSymbol(curr_symb);
        }

        //std::cout << "End file" << std::endl << std::endl << std::endl;
        g_stat.mergeNewStat(l_stat);
    
        current_index_file = next_unread_file.fetch_add(1);
    }
}

void writeMostFreqSymb(stat::GeneralStat& g_stat)
{
    auto arr_freq_let = g_stat.getMostFreqSymb();
    if (arr_freq_let[0] != '\0')
    {
        std::cout << "Most frequent symbols:" << std::endl;
        for (uint32_t i = 0; i < arr_freq_let.size() && arr_freq_let[i] != '\0'; ++i)
            std::cout << arr_freq_let[i] << std::endl;
    }
    else
        std::cout << "Symbols not found" << std::endl;
}

void writeMostRareSymb(stat::GeneralStat& g_stat)
{
    auto arr_rare_let = g_stat.getMostRareSymb();
    if (arr_rare_let[0] != '\0')
    {
        std::cout << "Rarest symbols:" << std::endl;
        for (uint32_t i = 0; i < arr_rare_let.size() && arr_rare_let[i] != '\0'; ++i)
            std::cout << arr_rare_let[i] << std::endl;
    }
    else
        std::cout << "Symbols not found" << std::endl;
}

void writeProbSymbol(stat::GeneralStat& g_stat)
{
    std::cout << "Enter symbol: ";
    char symbol = '\0';
    std::cin >> symbol;
    std::cout << "Probability '" << symbol << "' = ~" << g_stat.getChance(symbol) << std::endl;       
}

void interface_func(stat::GeneralStat& g_stat)
{
    std::cout << "Instruction: enter symbol" << std::endl;
    std::cout << " -'f' for see 5 most fequent symbols" << std::endl; 
    std::cout << " -'r' for see 3 rarest symbols" << std::endl; 
    std::cout << " -'c' for see probability symbol" << std::endl;
    std::cout << " -'q' for quit" << std::endl << std::endl;

    char user_request = '\0';
    while (user_request != 'q')
    {
        std::cout << "Enter your request: ";
        std::cin >> user_request;

        switch (user_request)
        {
            case 'f':
                writeMostFreqSymb(g_stat);
                break;
            case 'r':
                writeMostRareSymb(g_stat);
               break;
            case 'c':
                writeProbSymbol(g_stat);
                break;
            case 'q':
                break;
            default:
                std::cout << "Unknown request! Please, try again" << std::endl;
        }
    }       
}


int main()
{
    try
    {
        std::atomic<uint32_t> next_unread_file{0};        
        std::vector<std::string> filenames {"../test_files/first.py", "../test_files/init.vim", "../test_files/plug.vim", 
            "../test_files/user-dirs.dirs", "../test_files/.bash_history", "../test_files/.fish_history"};

        stat::GeneralStat g_stat;
        
        uint32_t count_reader_threads = askCount("Enter count reader threads: ");
        std::vector<std::thread> reader_threads;
        
        std::thread interface(interface_func, std::ref(g_stat));
        for (uint32_t i = 0; i < count_reader_threads; i++)
            reader_threads.push_back(std::move(std::thread(std::ref(reader_func), std::ref(filenames), std::ref(g_stat), 
                            std::ref(next_unread_file))));

        interface.join();
        std::cout << "Shutdown interface. Wait for shutdown readers..." << std::endl;
        for (auto& r_thread: reader_threads)
            r_thread.join();
                
        std::cout << "Shutdown program" << std::endl;

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Unrnown error" << std::endl;
        return EXIT_FAILURE;
    }
}
