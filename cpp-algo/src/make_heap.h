
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
        //adjust heap, so that we have hole of last node; mode  the hole_index to suitable position and then push value;

        //subtree of hole_index are all complete tree;

        for(int parent_index = hole_index; parent_index < (len-1)/2; parent_index++) {

        }
        
    } 

    void buildHeapByAdjust(int arr[], int len) {
        for(int parent_node_index = (len-1-1)/2 ; parent_node_index >= 0; parent_node_index--){
           adjustHeap(arr, len, parent_node_index, arr[parent_node_index]);
        }
    }

};
