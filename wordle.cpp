// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generator(std::string::size_type index, std::string::size_type length, const std::string& in, const std::string& floating, const std::string& shortFloating, size_t unknownLet, std::string& output, std::set<std::string>& possibilities);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> tempOutput;
    std::set<std::string> output;
    std::string temp;
    //setup temp string
    for(std::string::size_type i = 0; i < in.size(); ++i)
    {
        temp += 'a';
    }

    size_t count = 0;
    for(size_t i = 0; i < in.length(); ++i)
    {
        if(in[i] == '-') ++count;
    }

    size_t unknowns = count - floating.length();
    generator(0, in.size(), in, floating, floating, unknowns, temp, tempOutput);

    std::set<std::string>::iterator it = tempOutput.begin();
    while(it != tempOutput.end())
    {
        if(dict.find(*it) != dict.end())
        {
            output.insert(*it);
        }
        ++it;
    }
    
    return output;
    
}

// Define any helper functions here

//add all possible permutations to output
void generator(std::string::size_type index, std::string::size_type length, const std::string& in, const std::string& floating, const std::string& shortFloating, size_t unknownLet, std::string& output, std::set<std::string>& possibilities)
{
    //add all possible permutations to output 

    //got to the end
    if(index == length)
    {
        //std::cout << "got to end" << std::endl;
        //checking to see if it has the floating characters

        //MUST FIX: accounting for duplicate characters
        std::string temp = output;
        bool condition = true;
        for(std::string::size_type i = 0; i < floating.size(); ++i)
        {
            //does not have floating character
            std::string::size_type pos = temp.find(floating[i]);
            if(pos == std::string::npos)
            {
                condition = false;
            }
            else
            {
                temp[pos] = ' ';
            }
        }

        //if the string has all floating characters then insert it
        if(condition)
        {
            possibilities.insert(output);
        }

    }
    else
    {
        //does not have a guaranteed character
        if(in[index] == '-')
        {
            //new idea of try the floating characters first before trying all other characters
            std::string temp = shortFloating;
            for(size_t i = 0; i < shortFloating.size(); ++i)
            {
                output[index] = shortFloating[i];
                temp = shortFloating.substr(0,i) + shortFloating.substr(i+1, shortFloating.length() - i - 1);

                generator(index+1, length, in, floating, temp, unknownLet, output, possibilities);
            }
            //at this point have tried available floating characters all of which did not work

            //pass the same set of shortened floating characters to next level as we started with
            if(unknownLet > 0)
            {
                for(std::string::size_type i = 0; i < 26; ++i)
                {
                    if(shortFloating.find((char) (i + 97)) == std::string::npos)
                    {
                        output[index] = (char) (i + 97);
                        generator(index + 1, length, in, floating, shortFloating, unknownLet - 1, output, possibilities);
                    }
                }
            }
            
        }
        //does have guaranteed character
        else
        {
            output[index] = in[index];
            generator(index + 1, length, in, floating, shortFloating, unknownLet, output, possibilities);
        }
        
    }


    
}

