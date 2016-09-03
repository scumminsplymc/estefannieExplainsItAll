#include <iostream>
#include <array>
using namespace std;

void merge(int array[], int startIndex, int endIndex)
{
    int arraySize = (endIndex - startIndex) + 1;
    int *tempArray = new int [arraySize]();

    int i = startIndex;
    int mid = (startIndex + endIndex)/2;
    int counter = 0;
    int j = mid + 1;

    while (counter < arraySize)
    {
        if((i<=mid) && (array[i] < array[j]))
        {
            tempArray[counter++] = array[i++];
        }
        else
        {
            tempArray[counter++] = array[j++];
        }

    }

    for(int i=0; i < arraySize; i++)
    {
        array[startIndex+i] = tempArray[i];
    }

    delete []tempArray;

}

void merge_sort(int iArray[], int startIndex, int endIndex)
{
    int midIndex;

    //Check for base case
    if (startIndex >= endIndex)
    {
        return;
    }

    //First, divide in half
    midIndex = (startIndex + endIndex)/2;

    //First recursive call
    merge_sort(iArray, startIndex, midIndex);

    //Second recursive call
    merge_sort(iArray, midIndex+1, endIndex);

    //The merge function
    merge(iArray, startIndex, endIndex);

}

int main(int argc, char *argv[])
{
    int iArray[10] = {2,5,6,4,7,2,8,3,9,10};

    merge_sort(iArray, 0, 9);

    //Print the sorted array
    for(int i=0; i < 10; i++)
    {
        cout << iArray[i] << endl;
    }

    return 0;
}
