

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t row,
    size_t col);

bool isValidWorker(unsigned int worker, size_t row, const AvailabilityMatrix& avail, DailySchedule& sched);
bool isValidSchedule(const size_t maxShifts, const AvailabilityMatrix& avail, DailySchedule& sched);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    return scheduleHelper(avail,dailyNeed,maxShifts,sched,0,0);


}


//rows are days; cols are workers
bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t row,
    size_t col)
{
    //completely filled the schedule matrix now check to see if it is valid
    if(row > sched.size())
    {
        return isValidSchedule(maxShifts, avail, sched);
    }

    bool temp = false;
    for(size_t i = 0; i < avail[row].size(); ++i)
    {
        //setting one of the slots to a valid worker
        //valid if it is in the avail matrix and not already working that day
        if(isValidWorker(i, row, avail, sched));
        {
            sched[row][col] = i;

            //properly going to the next location
            if(col + 1 < dailyNeed)
            {
                temp = scheduleHelper(avail, dailyNeed, maxShifts, sched, row, col + 1);
            }
            else
            {
                temp = scheduleHelper(avail, dailyNeed, maxShifts, sched, row + 1, 0);
            }
        }
        
        if(temp == true)
        {
            return true;
        }
        
    }
    return false;
}


//checks to see if it is a valid worker for the day
bool isValidWorker(unsigned int worker, size_t row, const AvailabilityMatrix& avail, DailySchedule& sched)
{
    //check to see if in availability matrix
    if(avail[row][worker] == false) return false;

    //check to see if the worker is already working that day
    std::vector<Worker_T>::iterator it = sched[row].begin();
    while(it != sched[row].end())
    {
        if(*it == worker) return false;
        ++it;
    }

    return true;
}

//check to see if it is a valid completed schedule
//it is a valid schedule if the workers are working less than or equal to the max shifts
bool isValidSchedule(const size_t maxShifts, const AvailabilityMatrix& avail, DailySchedule& sched)
{
    //setup counter for how many times each worker is working
    size_t size = avail[0].size();
    std::vector<int> count;
    for(size_t i = 0; i < size; ++i) count.push_back(0);

    for(size_t i = 0; i < sched.size(); ++i)
    {
        std::vector<Worker_T>::iterator it = sched[i].begin();
        while(it != sched[i].end())
        {
            ++count[*it];
            if(count[*it] > maxShifts) return false;
        }
    }

    return true;

}