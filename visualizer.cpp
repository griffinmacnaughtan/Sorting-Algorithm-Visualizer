#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
#include<stdlib.h>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 750;

const int arrSize = 120;
const int rectSize = 10;

int arr[arrSize];
int Barr[arrSize];
int speed;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Function declarations to keep track of which are added
void bubbleSort();
void bogoSort(int arr[], int n);
bool isSorted(int arr[], int n);
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
int n = sizeof(arr) / sizeof(arr[0]);
void insertionSort(int arr[], int size);
void mergeSort(int arr[], int start, int end);
void merge(int arr[], int left, int mid, int right);
void quickSort(int arr[], int start, int end);
int partition(int arr[], int start, int end);
void selectionSort(int arr[], int n);

bool complete = false;

bool create() {

    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else {
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))) {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = { i, 0, rectSize, arr[j] };
        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 40, 95, 218, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % (SCREEN_HEIGHT);
        Barr[i] = random;
    }
}

void go()
{
    if (!create())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case(SDLK_ESCAPE):
                        cout << "\nEXITING SORTING VISUALIZER.\n";
                        exit(0);
                        break;
                    case(SDLK_SPACE):
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        break;
                    case(SDLK_b):
                        loadArr();
                        complete = false;
                        bubbleSort();
                        complete = true;
                        break;
                    case(SDLK_h):
                        loadArr();
                        complete = false;
                        heapSort(arr, n);
                        complete = true;
                        break;
                    case(SDLK_i):
                        loadArr();
                        complete = false;
                        insertionSort(arr, arrSize);
                        complete = true;
                        break;
                    case(SDLK_m):
                        loadArr();
                        complete = false;
                        mergeSort(arr, 0, arrSize - 1);
                        complete = true;
                        break;
                    case(SDLK_q):
                        loadArr();
                        complete = false;
                        quickSort(arr, 0, arrSize - 1);
                        complete = true;
                        break;
                    case(SDLK_s):
                        loadArr();
                        complete = false;
                        selectionSort(arr, arrSize);
                        complete = true;
                        break;
                    case(SDLK_1):
                        speed = 5;
                        break;
                    case(SDLK_2):
                        speed = 25;
                        break;
                    case(SDLK_0):
                        loadArr();
                        complete = false;
                        bogoSort(arr, arrSize);
                        complete = true;
                        break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controlInfo()
{
    cout << "--> Press the space bar to Generate a random list.\n"
        << "--> Press b to use Bubble Sort.\n"
        << "--> Press h to use Heap Sort.\n"
        << "--> Press i to use Insertion Sort.\n"
        << "--> Press m to use Merge Sort.\n"
        << "--> Press q to use Quick Sort.\n"
        << "--> Press s to use Selection Sort.\n"
        << "--> Press 0 to use Bogo Sort (Warning - No way to exit program once this has started - unlikely to successfully sort.\n"
        << "--> Press 1 for faster sort speed.\n"
        << "--> Press 2 for slower sort speed.\n"
        << "--> Press esc to exit the program.\n\n"
        << "Press enter to begin!\n\n";

    char a[100];
    cin.getline(a, 100);
    if (a == "\n") {
        return true;
    }
}

void introMessage() {
    cout << "Welcome to the Sorting Algorithm Visualizer! \n"
         << "This program is an interactive way of showing the styles and efficacies of popular sorting algorithms.\n"
         << "Descriptions of each algorithm can be found in the README.md file\n\n"
         << "Created by Griffin MacNaughtan - July 2022 \n\n"
         << "All code written in C++, UI created using SDL2. Visualizer inspired by Dipesh Mann \n https://github.com/dipesh-m/Sorting-Visualizer \n"
         << "Press enter to see controls!";

    char a[100];
    cin.getline(a, 100);
    if (a == "\n") {
        return;
    }
}

int main(int argc, char* args[]) {
    introMessage();
    controlInfo();
    while (1) {
        go();
    }

    return 0;
}

//Different Algorithms. More coming soon!

void bogoSort(int arr[], int n)
{
    // if array is not sorted then shuffle
    // the array again
    while (!isSorted(arr, n)) {
        for (int i = 0; i < n; i++) {
            int a = rand() % n;
            swap(arr[i], arr[a]);
            visualize(i, a, arrSize - 1);
        }
        SDL_Delay(speed);
    }
        
}

bool isSorted(int arr[], int n)
{
    while (--n > 1)
        if (arr[n] < arr[n - 1])
            return false;
    return true;
}

void bubbleSort() {
    int i, j;
    for (i = 0; i < arrSize - 1; i++) {
        for (j = 0; j < arrSize - 1 - i; j++) {
            if (arr[j + 1] < arr[j]) {
                swap(arr[j], arr[j + 1]);

                visualize(j + 1, j, arrSize - 1);
            }
            SDL_Delay(speed);
        }
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        visualize(largest, i);
        SDL_Delay(speed);
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
        }
}

void heapSort(int arr[], int n) { 
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

void insertionSort(int arr[], int size)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;

            visualize(i, j + 1);
            SDL_Delay(speed);
        }
        arr[j + 1] = key;
    }
}

void mergeSort(int array[], int start, int end)
{
    if (start>= end)
        return;

    int mid = start + (end - start) / 2;
    mergeSort(array, start, mid);
    mergeSort(array, mid + 1, end);
    merge(array, start, mid, end);
}

void merge(int array[], int left, int mid,
    int right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    int* leftArray = new int[subArrayOne],
        * rightArray = new int[subArrayTwo];

    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    int indexOfSubArrayOne
        = 0,
        indexOfSubArrayTwo
        = 0;
    int indexOfMergedArray
        = left;

    while (indexOfSubArrayOne < subArrayOne
        && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            visualize(indexOfSubArrayOne, indexOfSubArrayTwo);
            SDL_Delay(speed);
        }
        else {
            array[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
            visualize(indexOfMergedArray, indexOfSubArrayOne);
            SDL_Delay(speed);
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray]
            = leftArray[indexOfSubArrayOne];
        visualize(-1, indexOfSubArrayOne);
        SDL_Delay(speed);
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray]
            = rightArray[indexOfSubArrayTwo];
        visualize(-1, indexOfSubArrayTwo);
        SDL_Delay(speed);
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void quickSort(int arr[], int start, int end) {
    if (start >= end)
        return;

    int x = partition(arr, start, end);

    quickSort(arr, start, x - 1); //Left

    quickSort(arr, x + 1, end); //Right
}

int partition(int arr[], int start, int end) {
    
    int pivot = arr[start];
    
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }

    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);
    visualize(pivotIndex, start);
    SDL_Delay(speed);

    int i = start, j = end; //L&R

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
            visualize(i, j);
            SDL_Delay(speed);
        }
    }

    return pivotIndex;
}

void selectionSort(int arr[], int n) {
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) 
    {
        min_idx = i;

        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
                visualize(i, min_idx);
                SDL_Delay(speed);
            }
            
        }
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}
