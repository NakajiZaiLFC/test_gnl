#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "get_next_line.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

void create_test_file(const char *filename, const char *content)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1)
    {
        write(fd, content, strlen(content));
        close(fd);
    }
}

void test_file(const char *filename, const char *test_name)
{
    int     fd;
    char    *line;
    int     line_count = 0;

    printf("\n%s=== Testing %s ===%s\n", YELLOW, test_name, RESET);
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("%sError opening file%s\n", RED, RESET);
        return;
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", ++line_count, line);
        free(line);
    }
    
    close(fd);
    printf("%sCompleted: Read %d lines%s\n", GREEN, line_count, RESET);
}

int main(void)
{
    printf("Starting Get Next Line Tests (BUFFER_SIZE: %d)\n", BUFFER_SIZE);

    // テストケース1: 基本的な複数行
    create_test_file("test1.txt", "Hello, World!\nThis is a test.\nLast line.");
    test_file("test1.txt", "Basic multiple lines");

    // テストケース2: 空行を含む
    create_test_file("test2.txt", "First line\n\nThird line\n\nFifth line");
    test_file("test2.txt", "Empty lines");

    // テストケース3: 1行のみ（改行なし）
    create_test_file("test3.txt", "Single line without newline");
    test_file("test3.txt", "Single line without newline");

    // テストケース4: 空ファイル
    create_test_file("test4.txt", "");
    test_file("test4.txt", "Empty file");

    // テストケース5: 改行のみ
    create_test_file("test5.txt", "\n\n\n");
    test_file("test5.txt", "Only newlines");

    // テストケース6: 長い行
    char *long_line = (char *)malloc(1000);
    memset(long_line, 'x', 999);
    long_line[999] = '\0';
    create_test_file("test6.txt", long_line);
    test_file("test6.txt", "Long line");
    free(long_line);

    // テストケース7: 異なる改行パターン
    create_test_file("test7.txt", "Line1\nLine2\nLine3\n");
    test_file("test7.txt", "Different newline patterns");

    // テストケース8: 存在しないファイル
    test_file("nonexistent.txt", "Non-existent file");

    // テストケース9: Unicode文字
    create_test_file("test9.txt", "こんにちは\n你好\nBonjour\n");
    test_file("test9.txt", "Unicode characters");

    // 後処理: テストファイルの削除
    unlink("test1.txt");
    unlink("test2.txt");
    unlink("test3.txt");
    unlink("test4.txt");
    unlink("test5.txt");
    unlink("test6.txt");
    unlink("test7.txt");
    unlink("test9.txt");

    printf("\n%sAll tests completed!%s\n", GREEN, RESET);
    return (0);
}
