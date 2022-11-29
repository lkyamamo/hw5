

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
    size_t col,
    std::vector<size_t>& count);

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

    std::vector<size_t> workCounter;
    for(size_t i = 0; i < avail[0].size(); ++i)
    {
        workCounter.push_back(0);
    }

    return scheduleHelper(avail,dailyNeed,maxShifts,sched,0,0,workCounter);


}


//rows are days; cols are workers
bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t row,
    size_t col,
    std::vector<size_t>& count)
{
    //completely filled the schedule matrix now check to see if it is valid
    if(row > sched.size() - 1)
    {
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
            //NEW IDEA not only check to see if worker is already working that day, but have counter in place
            //to see if they already are working the max amount of days

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

            //check to see if he is already working max amount of days
            if(count[i] == maxShifts) condition = false;
        }

        //if it is a valid worker
        if(condition == true)
        {
            sched[row][col] = i;
            ++count[i];

            //properly going to the next location
            if(col + 1 < dailyNeed)
            {
                temp = scheduleHelper(avail, dailyNeed, maxShifts, sched, row, col + 1, count);
            }
            else
            {
                temp = scheduleHelper(avail, dailyNeed, maxShifts, sched, row + 1, 0, count);
            }
        }
        
        if(temp == true)
        {
            return true;
        }
        --count[i];
    }
    sched[row][col] = INVALID_ID;
    return false;
}