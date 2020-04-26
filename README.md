# url_extend
ptrace extension to enable applications to access remote URLs as if they were local files


URL extender:
To compile the file: 
```sh
make
```
To delete the executable: 
```sh
make clean 
```
To try a test case:
```sh
./url_extend application_name url
```

Test cases tried:
```sh
./url_extend wc http://www.cs.stonybrook.edu
./url_extend wc http://scratchpads.eu/explore/sites-list 
./url_extend vim http://www.cs.stonybrook.edu
./url_extend head http://www.cs.stonybrook.edu
./url_extend tail http://www.cs.stonybrook.edu
./url_extend nano http://www.cs.stonybrook.edu
./url_extend cat http://www.cs.stonybrook.edu
./url_extend grep -n College http://www.cs.stonybrook.edu 
./url_extend ls http://www.cs.stonybrook.edu
./url_extend head -3 http://www.cs.stonybrook.edu | cat 
./url_extend cat http://www.cs.stonybrook.edu | grep -n html
./url_extend cat http://www.cs.stonybrook.edu >> abc.html
./url_extend cat http://www.cs.stonybrook.edu > abc.html
./url_extend vi http://www.cs.stonybrook.edu
```

# Details:
1. Url is given in the format of http:// or https://
2. Url is downloaded as a temporary read-only file in the /tmp/url_extend_madhu folder as index.html every time a URL is accessed by an application.
This file is replaced by the newly downloaded URL index.html file every time an URL is accessed.
3. I have intercepted the system calls for stat64, lstat64, open and openat.
