#include <iostream>

/*
    0    
 1      2
3 4    5 6
a heap represent as a array arr[], index i from 0
node arr[i]'s property
left node arr[2i+1]; right node arr[2i+2];
parent  arr[(i-1)/2]
*/
class HeapTool{

public:
    bool debug_output;
    HeapTool(){
        debug_output = false;
    }
    void recusiveBuildHeapOfNode(int arr[], int len, int parenet_index)
    {
       int left_child_index = 2 * parenet_index + 1;
       int right_child_index = 2 * parenet_index + 2;
       int largest_num_index = parenet_index;
       if( left_child_index < len && arr[left_child_index] > arr[largest_num_index] ) {
           largest_num_index = left_child_index;
       } 
       if( right_child_index < len && arr[right_child_index] > arr[largest_num_index] ) {
           largest_num_index = right_child_index;
       } 
       if(largest_num_index != parenet_index) {
           int x = arr[largest_num_index];
          arr[largest_num_index] = arr[parenet_index];
        arr[parenet_index] = x;
        recusiveBuildHeapOfNode(arr, len, largest_num_index);
    }
   }


    void recusiveBuildHeap(int arr[], int len)
    {
        for(int parent_node_index = (len-1-1)/2 ; parent_node_index >= 0; parent_node_index--){
           recusiveBuildHeapOfNode(arr, len, parent_node_index);
        }
    }

    void adjustHeap(int arr[], int len, int hole_index, int value) {
        //adjust heap, so that we have hole of last node; mode  the hole_index to suitable position and then push value
        const int top_index = hole_index;
        //assume subtree of hole_index are all complete tree;
        //now we delete data which located in hole_index,
        // (len-1)/2 , so those parent node have both left and right nodes;
        for(int parent_index = hole_index; parent_index < (len-1)/2; ) {
             int left_child_index  =  2*parent_index+1; 
             int right_child_index = left_child_index + 1; 
             if(arr[left_child_index] > arr[right_child_index]) { 
                  arr[hole_index] = arr[left_child_index];
                  hole_index = left_child_index;
             } else {
                  arr[hole_index] = arr[right_child_index];
                  hole_index = right_child_index;
             }
             parent_index = hole_index;
        }
        //last node with onle one leaf
        if( (len&1) == 0 && hole_index == (len-2)/2) {
             int left_child_index  =  2*hole_index+1;
             arr[hole_index] = arr[left_child_index];
             hole_index = left_child_index;
        }
        // and push new value to tree
        
        pushHeap(arr, len, top_index, hole_index, value);
        
        
    } 
    void pushHeap(int arr[], int len, int top_index, int hole_index, int value) {
        if(debug_output) {
            outArray(arr, len);
            std::cout <<"\n hold_index:" << hole_index <<  ", value:" << value << ", top_index:" << top_index << "\n";
        }
        //compare ancestor by step, find a suitable hole_index
        for(int parent_index = (hole_index-1)/2; hole_index > top_index ;) {
            if(arr[parent_index] < value) {
                arr[hole_index] = arr[parent_index];
                hole_index = parent_index;
                parent_index = (hole_index -1)/2;
            } else {
                break;
            }
        }
        arr[hole_index] = value;
    }


    void buildHeapByAdjust(int arr[], int len) {

        if(debug_output) {
             std::cout << "\nInput Arr: ";
             for(int j = 0; j < len; j++) {
                 std::cout << arr[j] <<",";
             }
        }


        for(int parent_node_index = (len-1-1)/2 ; parent_node_index >= 0; parent_node_index--){
           adjustHeap(arr, len, parent_node_index, arr[parent_node_index]);
           if(debug_output) {
             outArray(arr, len);
             std::cout << "     parent_nodex_index=" << parent_node_index << "\n";
          }
        }
    }
 
    void outArray(int arr[], int len) {
        std::cout << "\nAdjustArr: ";
             for(int j = 0; j < len; j++) {
                 std::cout << arr[j] <<",";
        }
    }
    

};
