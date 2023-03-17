#include "timer.hpp"
#include "add_func.hpp"

#include <thread>

int main(int argc, char** argv)
{
    try
    {
        int rank, size;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        std::cout << "Start process " << rank << " of total count " << size << std::endl;
        
        if (rank == 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            main_runtime_func(size);
        }
        else
            worker_runtime_func(0);

        std::cout << "End process " << rank << " of total count " << size << std::endl;
        MPI_Finalize();
        
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
