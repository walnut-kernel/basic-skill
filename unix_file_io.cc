

/**
 * 文件描述符       用途        POSIX名称       stdio流
 *     0           标准输入     STDIN_FILENO    stdin
 *     1           标准输出     STDOUT_FILENO   stdout
 *     2           标准错误     STDERR_FILENO   stderr
 * 在程序中指代这些文件描述符时，可以使用数字（0、1、2）表示，或者采用<unistd.h>所定义的POSIX标准名称——此方法更为可取。
 * 
 * UNIX I/O模型的显著特点之一是其输入/输出的通用性概念。这意味着使用4个同样的系统调用open()、read()、write()和close()可以对所有类型的文件执行I/O操作，包括终端之类的设备。
 * 要实现通用I/O，就必须确保每一文件系统和设备驱动程序都实现了相同的I/O系统调用集。由于文件系统或设备所特有的操作细节在内核中处理，在编程时通常可以忽略设备专有的因素。
 * 一旦应用程序需要访问文件系统或设备的专有功能时，可以选择瑞士军刀般的ioctl()系统调用，该调用为通用I/O模型之外的专有特性提供了访问接口。
 * 
 * int open(const char *pathname, int flags, mode_t mode);
 *     pathname：文件名
 *     flags：访问模式
 *         The argument flags must include one of the following access modes:
 *             O_RDONLY    以只读方式打开文件
 *             O_WRONLY    以只写方式打开文件
 *             O_RDWR      以读写方式打开文件
 *         In addition, zero or more file creation flags and file status flags can be bitwise-or'd in flags.
 *     mode：访问权限；若flafs中未指定O_CREAT，可省略此参数
 * 
 *     On success, the file descriptor (a nonnegative integer) returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.
 *     On error, -1 is returned, and errno is set appropriately.
 *     
 * ssize_t read(int fd, void *buf, size_t count);
 *     fd：已打开的文件
 *     buf：存放读取的数据的内存缓冲区地址，缓冲区至少应有count个字节
 *     count：最多能读取的字节数
 * 
 *     On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number.  
 *     It is not an error if this number is smaller than the number of  bytes requested;  
 *     this may happen for example because fewer bytes are actually available right now (maybe because we were close to end-of-file, 
 *     or because we are reading from a pipe, or from a terminal), or because read() was interrupted by a signal.
 *     On error, -1 is returned, and errno is set appropriately.  In this case, it is left unspecified whether the file position (if any) changes.
 * 
 * ssize_t write(int fd, const void *buf, size_t count);
 *     fd：已打开的文件
 *     buf：要写入文件中数据的内存地址
 *     count：要从buffer写入文件的数据字节数
 * 
 *     The number of bytes written may be less than count if, for example, there is insufficient space on the underlying physical medium, or the RLIMIT_FSIZE resource limit is encountered,
 *     or the call was interrupted by a signal handler after having written less than count bytes.
 * 
 *     On success, the number of bytes written is returned, and the file position is advanced by this number.  
 *     On error, -1 is returned, and errno is set to indicate the cause of the error. In this case, it is left unspecified whether the file position (if any) changes.
 * 
 * int close(int fd);
 *     fd：已打开的文件
 * 
 *     close() closes a file descriptor, so that it no longer refers to any file and may be reused.
 *     close() returns zero on success.  On error, -1 is returned, and errno is set appropriately.
 * 
 * off_t lseek(int fd, off_t offset, int whence);
 *     对于每个打开的文件，系统内核会记录其文件偏移量，有时也将文件偏移量称为读写偏移量或指针。
 *     文件偏移量是指执行下一个read()或write()操作的文件起始位置，会以相对于文件头部起始点的文件当前位置来表示。文件第一个字节的偏移量为0。
 *     文件打开时，会将文件偏移量设置为指向文件开始，以后每次read()或write()调用将自动对其进行调整，以指向已读或已写数据后的下一字节。因此，连续的read()和write()调用将按顺序递进，对文件进行操作。
 *     fd：已打开的文件
 *     offset：以字节为单位的数值
 *     whence：参照哪个基点来解释offset参数
 *         SEEK_SET    将文件偏移量设置为从文件头部起始点开始的offset个字节
 *         SEEK_CUR    相对于当前文件偏移量，将文件偏移量调整offset个字节
 *         SEEK_END    将文件偏移量设置为起始于文件尾部的offset个字节。也就是说，offset参数应该从文件最后一个字节之后的下一个字节算起
 *         如果whence参数值为SEEK_CUR或SEEK_END，offset参数可以为正数也可以为负数；如果whence参数值为SEEK_SET，offset参数值必须为非负数。
 *     lseek()调用只是调整内核中与文件描述符相关的文件偏移量记录，并没有引起对任何物理设备的访问。
 *     Upon  successful completion, lseek() returns the resulting offset location as measured in bytes from the beginning of the file.  
 *     On error, the value (off_t) -1 is returned and errno is set to indicate the error.
 * 
*/