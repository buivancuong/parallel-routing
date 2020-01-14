#### 1. Process finished with exit code 139 (interrupted by signal 8: SIGFPE)

Lỗi này gây ra do truy cập vào vùng nhớ không đúng:

* Chưa có default constructor cho ```Node::Node()```.
* ```std::vector``` chưa có phần tử nào, nên không thể random access.
* ```this->locality[0].insert(nearNeighbor);```: mới khai báo locality, nhưng chưa khởi tạo nó, nên ```locality``` chưa có phần tử ```locality[0]```. 
Có thể khắc phục bằng việc tạo 1 container mới trống vào ngay sau ```locality``` bằng lệnh ```this->locality.emplace_back();```
* Với một đối tượng STL có cấu trúc phức tạp và kích thước lớn, phải tạo giá trị trung gian (nên tạo dưới dạng Stack variable để biến tự giải phóng khi out of scope): 
Ví dụ với một truy cập phải xuyên qua nhiều lần tham chiếu như ```for (std::pair<int, Node*> neighbor : lowerNeighbor.second->getLocality()[0]){};```,
ta nên tạo ra 1 biến trung gian ```localityOfLowerNeighbor``` trung vùng nhớ Stack thông qua khai khởi tạo ```std::map<int, Node*> localityOfLowerNeighbor (lowerNeighbor.second->getLocality()[0]);```.


#### 2. Dùng quá ngốn RAM

* Lỗi này gây ra do sử dụng thuần variable cho các đối tượng của các class. Khắc phục bằng cách sử dụng pointer (con trỏ) thay cho variable.
* Lỗi này cũng có thể đi kèm với việc chương trình bị treo, khi debug sẽ gặp tín hiệu **SIGSTOP** từ OS. Đây là lỗi không thể ```try catch```,
do đó, bắt buộc phải dựa vào debuger để tìm ra dòng lệnh gây infinity loop. 


#### 3. CLION không cho phép chạy đa luồng vì khi build gặp lỗi **```undefined reference to pthread_create```**.

* Đang tìm hiểu nguyên nhân. 
* Lỗi này có thể khắc phục bằng cách thêm đoạn lệnh sau vào file **CMakeLists.txt** của project.
```cmake 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
```

