# fixed_capacity_string
A fixed capacity string type, allocated on the stack.
Useful when you know the limits of your string data, and don't want to use the heap.

### Usage:
```
auto str = fixed_capacity_string<5>::from_raw("abcdef");
std::cout << str.sv();
```
Outputs:
```
abcde
```