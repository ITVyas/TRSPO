#include <iostream>
#include <math.h>
#include "mpi.h"
#include <stdlib.h>
#include <chrono>


bool isInCircle(double x, double y)
{
    return pow(x, 2) + pow(y, 2) <= 1;
}

long countAmountInCircle(double* pointsX, double* pointsY, long N)
{
    long amount = 0;

    for (long i = 0; i < N; i++)
        if (isInCircle(pointsX[i], pointsY[i]))
            amount++;

    return amount;
}

double countPi(double* pointsX, double* pointsY, long N)
{
    long inCircleAmount = countAmountInCircle(pointsX, pointsY, N);
    return ((double)inCircleAmount) / N * 4;
}

void printOutResult(double* calculatedPi, double* timeCalculation, int N, long pointsAmount)
{
    for (int i = 0; i < N; i++)
    {
        std::cout << "Points: " << pointsAmount * (i+1) / N  << "\t| Pi = " << calculatedPi[N - 1 - i] << "\t| Time: " << timeCalculation[N - 1 - i] << "s" << std::endl;
    }
}


int main(int argc, char** argv)
{
    srand(time(0));
    int ProcRank, ProcNum;
    const long N = 10000000;
    
    double* pointsX;
    double* pointsY; 

    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* calculatedPi = new double[ProcNum];
    double* timeCalculation = new double[ProcNum];

    int NThisProc = N * (ProcNum - ProcRank) / ProcNum ;
    pointsX = new double[NThisProc];
    pointsY = new double[NThisProc];

    if (ProcRank == 0)
    {
        for (long i = 0; i < N; i++)
        {
            pointsX[i] = rand() / double(RAND_MAX);
            pointsY[i] = rand() / double(RAND_MAX);
        }   

        for (int i = 0; i < ProcNum - 1; i++)
        {
            int part = N/ProcNum*(i+1);
            MPI_Send(pointsX, part, MPI_DOUBLE, ProcNum - 1 - i, 0, MPI_COMM_WORLD);
            MPI_Send(pointsY, part, MPI_DOUBLE, ProcNum - 1 - i, 1, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(pointsX, NThisProc, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(pointsY, NThisProc, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    double pi = countPi(pointsX, pointsY, NThisProc);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    double duration = diff.count();
    
    if (ProcRank != 0)
    {
        MPI_Send(&pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&duration, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        calculatedPi[0] = pi;
        timeCalculation[0] = duration;
        for (int i = 1; i < ProcNum; i++)
        {
            MPI_Recv(&(calculatedPi[i]), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&(timeCalculation[i]), 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        
        printOutResult(calculatedPi, timeCalculation, ProcNum, N);
    }
    
    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();




    return 0;

}
