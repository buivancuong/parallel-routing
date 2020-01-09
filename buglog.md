#### 1. Process finished with exit code 136 (interrupted by signal 8: SIGFPE)

Lỗi này gây ra do truy cập vào vùng nhớ không đúng:

* Chưa có default constructor cho ```Node::Node()```. $$
* ```std::vector``` chưa có phần tử nào, nên không thể random access.
* ```this->locality[0].insert(nearNeighbor);```: mới khai báo locality, nhưng chưa khởi tạo nó, nên ```locality``` chưa có phần tử ```locality[0]```. Có thể khắc phục bằng việc tạo 1 container mới trống vào ngay sau ```locality``` bằng lệnh ```this->locality.emplace_back();```

#### 2. Dùng quá ngốn RAM

Lỗi này gây ra do sử dụng thuần variable cho các đối tượng của các class. Khắc phục bằng cách sử dụng pointer (con trỏ) thay cho variable.


