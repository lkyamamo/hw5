

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

    //create initial sched matrix filled with 0
    for(size_t i = 0; i < avail.size(); ++i)
    {
        std::vector<Worker_T> temp;
        sched.push_back(temp);
        for(size_t j = 0; j < dailyNeed; ++j)
        {
            sched[i].push_back(INVALID_ID);
        }
    }

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
    if(row > sched.size() - 1)
    {
        //setup counter for how many times each worker is working
        size_t size = avail[0].size();
        std::vector<size_t> count;
        for(size_t i = 0; i < size; ++i) count.push_back(0);

        for(size_t i = 0; i < sched.size(); ++i)
        {
            std::vector<Worker_T>::iterator it = sched[i].begin();
            while(it != sched[i].end())
            {
                ++count[*it];
                if(count[*it] > maxShifts) return false;
                ++it;
            }
        }

        return true;
    }

    bool temp = false;
    for(size_t i = 0; i < avail[row].size(); ++i)
    {
        //setting one of the slots to a valid worker
        //valid if it is in the avail matrix and not already working that day
        bool condition;
        if(avail[row][i] == false) condition = false;
        else
        {
            //check to see if the worker is already working that day
            std::vector<Worker_T>::iterator it = sched[row].begin();
            while(it != sched[row].end())
            {
                if(*it == i)
                {
                    condition = false;
                    break;
                } 
                ++it;
            }
            if(it == sched[row].end()) condition = true;
        }

        //if it is a valid worker
        if(condition == true)
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
    sched[row][col] = INVALID_ID;
    return false;
}