```Bash
git clone --recursive git@github.com:RoyBellingan/boostIntrusive.git
cd boostIntrusive/
cmake .
make -j 8
./BoostJsonIntrusive
```

It should produce

```

------
default msg : source composite size does not match target size
expanded msg: the key >> timestamp << is non optional and missing in path /0/comment/0

------
default msg : source composite size does not match target size
expanded msg: the key >> author << is non optional and missing in path /0/comment/0/likes/1

------
default msg : not a number
expanded msg: Last json pointer was /0/comment/0/timestamp

```
