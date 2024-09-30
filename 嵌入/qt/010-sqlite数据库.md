
## 数据库
### 什么是数据库：
是按照数据结构来组织、存放和管理数据的建立在计算机存储设备上的数据仓库。  

另外：是本身可以视为电子化的文件柜——存储文件的地方。  
用户可以对其进行：新增、删除、查找、修改（更新）    
常见的数据库有层次、网状、时序模型、关系型数据库等。  
### 常见数据库
- MySQL ：（Oracle）是开源的、一个关系数据库管理系统、服务器是一个快速的、可靠的、易于使用的（1万~1000万）
- SQL Server：（微软）web上最流行的、主要在windows上运行、比较依赖操作系统、数据访问安全、有强大的web和安全的应用管理、操作比较简单、界面友好（自动增长）
- Oracle：（甲骨文）兼容性好、可移植性、可联接性强、生产效率高（有很多工具）、开放性（1000万~10亿）
- Sybase：（Sybase）基于客户/服务器结构的数据库、真正开放的、性能高（1万~100万）
- DB2：（IBM）大型企业应用（可以达到20亿）
- sqlite：轻量级的开源数据库、很适合应用于嵌入式设备（本地，100~1万）
### 数据库和数据库管理软件
- 数据库管理软件：为数据库而设计的电脑软件系统，一般具有存储、截取、安全保障备份等基本功能。
### 选择数据库
- Oracle：最贵的、功能最多（安装配置更麻烦）、跨平台性一般、不开源、速度最慢、最安全、开发难度一般
- MySQL/SQl Server：价格中等（有免费）、功能最少（安装比较方便）、跨平台性不好、不开源、速度中等、安全中等、开发难度相对简单
- sqlite：免费的、功能一般多（安装特方便）、嵌入式设备中最适合、开源的、速度最快、最不安全、开发简单
## sqlite基础
### 什么是sqlite
是一个进程内的数据库，实现了自给自足、无服务器、零配置、事务性的SQL数据引擎，不是独立于进程，可以通过软件和程序去使用这个数据库和访问文件。
### 特性
- 不需要配置、不需要管理和安装
- sqlite文件是存在一个单一的磁盘文件
- 非常轻量级，完全配置小于400kb，如果省略某些功能大约只有250kb
- 可以在unix（linux、mac、OS-X、Android、IOS）和windows（win32、winCE、winPT）
### sqlite数据类型
- NULL值
- integer整型
- real浮点型
- text字符串
 
## 安装sqlite3
### 在Ubuntu中直接安装：
```shell
sudo apt-get install sqlite
```
### 从官网去下载源码，编译安装
- 下载 https://www.sqlite.org/download.html
- 进入源码路径
```shell
shell.c  sqlite3.c  sqlite3ext.h  sqlite3.h
```
- 编译
```shell
gcc shell.c sqlite3.c -o sqlite3 -lpthread -ldl
```

### 图形界面的管理工具

- Linux：`sudo apt-get install sqlitebrowser`  
- windows：`SQLiteExpert_3.4.34.2256_XiaZaiBa.exe`

## sqlite3程序操作命令
点命令：以点开头、大小写区分的、末尾不需要加分号  
（1）在命令行：sqlite3 进入数据库的管理软件  
  sqlite3 数据库文件名（.db） 不存在就创建，存在就打开  
```c++
sqlite3 test.db
```
（2）在sqlite3的管理软件里面所有的命令都是 `.`开头  
`.database` 查看当前打开的数据库
（3）`.table`查看当前数据库的所有表格
创建表格  
（4）退出 `.quit` `.exit`  
（5）导出数据库  
在命令行: `sqlite3 数据库文件名 .dump > stu.sql`  
（6）导入数据库  
在命令行: `sqlite3 test.db < stu.sql`  
（7）备份数据库：.backup xxx.db
 
| 命令                         | 描述                                                                                                                                                                                                                        |
| ---------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| .backup ?DB? FILE            | 备份 DB 数据库（默认是 "main"）到 FILE 文件。                                                                                                                                                                               |
| .bail ON\|OFF                | 发生错误后停止。默认为 OFF。                                                                                                                                                                                                |
| .databases                   | 列出数据库的名称及其所依附的文件。                                                                                                                                                                                          |
| .dump ?TABLE?                | 以 SQL 文本格式转储数据库。如果指定了 TABLE 表，则只转储匹配 LIKE 模式的 TABLE 表。                                                                                                                                         |
| .echo ON\|OFF                | 开启或关闭 echo 命令。                                                                                                                                                                                                      |
| .exit                        | 退出 SQLite 提示符。                                                                                                                                                                                                        |
| .explain ON\|OFF             | 开启或关闭适合于 EXPLAIN 的输出模式。如果没有带参数，则为 EXPLAIN on，即开启 EXPLAIN。                                                                                                                                      |
| .header(s) ON         \| OFF | 开启或关闭头部显示。.help显示消息。                                                                                                                                                                                         |
| .import FILE TABLE           | 导入来自 FILE 文件的数据到 TABLE 表中。                                                                                                                                                                                     |
| .indices ?TABLE?             | 显示所有索引的名称。如果指定了 TABLE 表，则只显示匹配 LIKE 模式的 TABLE 表的索引。                                                                                                                                          |
| .load FILE ?ENTRY?           | 加载一个扩展库。                                                                                                                                                                                                            |
| .log FILE\|off               | 开启或关闭日志。FILE 文件可以是 stderr（标准错误）/stdout（标准输出）。                                                                                                                                                     |
| .mode MODE                   | 设置输出模式，MODE 可以是下列之一：csv 逗号分隔的值column 左对齐的列html HTML 的 `<table>` 代码insert TABLE 表的 SQL 插入（insert）语句line 每行一个值list 由 .separator 字符串分隔的值tabs 由 Tab 分隔的值tcl TCL 列表元素 |
| .nullvalue STRING            | 在 NULL 值的地方输出 STRING 字符串。                                                                                                                                                                                        |
| .output FILENAME             | 发送输出到 FILENAME 文件。                                                                                                                                                                                                  |
| .output stdout               | 发送输出到屏幕。                                                                                                                                                                                                            |
| .print STRING...             | 逐字地输出 STRING 字符串。                                                                                                                                                                                                  |
| .prompt MAIN CONTINUE        | 替换标准提示符。                                                                                                                                                                                                            |
| .quit                        | 退出 SQLite 提示符。                                                                                                                                                                                                        |
| .read FILENAME               | 执行 FILENAME 文件中的 SQL。                                                                                                                                                                                                |
| .schema ?TABLE?              | 显示 CREATE 语句。如果指定了 TABLE 表，则只显示匹配 LIKE 模式的 TABLE 表。                                                                                                                                                  |
| .separator STRING            | 改变输出模式和 .import 所使用的分隔符。                                                                                                                                                                                     |
| .show                        | 显示各种设置的当前值。.stats ON                                                                                                                                                                                             | OFF开启或关闭统计。 |
| .tables ?PATTERN?            | 列出匹配 LIKE 模式的表的名称。                                                                                                                                                                                              |
| .timeout MS                  | 尝试打开锁定的表 MS 毫秒。.width NUM NUM为 "column" 模式设置列宽度。                                                                                                                                                        |
| .timer ON\|OFF               | 开启或关闭 CPU 定时器。                                                                                                                                                                                                     |
## sql命令（各种数据库通用语言）
sql命令：所有sql命令以 ; 结尾、sql命令不区分大小写，SQL是一种专门用来与数据库通讯的语言。是目前应用最广泛的数据库语言。但是有些数据库对SQL做了修改，所有有时候可能会有不通用的情况  
- DDL - 数据定义语言   
命令描述CREATE创建一个新的表，一个表的视图，或者数据库中的其他对象。ALTER修改数据库中的某个已有的数据库对象，比如一个表。DROP删除整个表，或者表的视图，或者数据库中的其他对象。
- DML - 数据操作语言    
命令描述INSERT创建一条记录。UPDATE修改记录。DELETE删除记录。
- DQL - 数据查询语言    
命令描述SELECT从一个或多个表中检索某些记录。


### 创建表格  
```sql
create table info (num integer， name text， age integer， score real);
create table if not exists info1 (num integer primary key, name text not null, age integer null, score real null);
create table info2 (num integer primary key, name text not null default 'xxxx', age integer null  , score real null);
```
设置字段属性：（放在字段名和类型后面的，如果有多个属性依次往后添加）
- 主键 primary key：主键如果没有指定一般会帮你自动补一个，并且多条数据记录的主键不能一样
- 空与非空：null ， not null
- 默认值:default 值

### 添加数据
```sql
insert into info (num, name, age, score) values (1, 'wangda', 20, 100.0);
insert into info (num, name, age, score) values (2, "tianer", 21, 100.0);
insert into info (num, name, score) values (3, "zsan", 95.5);
insert into info (name, age, score) values ("lisi", 20, 80.0);
insert into info (name, age, score) values ("lisi", 20, 80.0);
insert into info (num, name, age, score) values (6, "wwu", 21, 88.0);
insert into info (num, name, age, score) values (7, "tianqi", 20, 90.0);
insert into "info2" (age, num, name, score)values(22, 10, 'haha', 60);
INSERT INTO "info" VALUES(1,'wangda',NULL,100.0);
INSERT INTO "info" VALUES(2,'liuer',21,88.0);
INSERT INTO "info1" VALUES(NULL,'lisi',21,92.0);
```
### 删除数据
```sql
delete from info where num=4;
delete from info where name='lisi';
```
### 查找数据
- 查看表格所有数据：select * from info;
- 查看n条数据：select * from info limit n;
- 升序：select * from info order by age asc;
- 降序：select * from info order by score desc;
- 查看数据记录个数:select count(*) from info;
- 条件查询:  
    ①where:  
    ```sql
    select * from info where age = 20;
    ```
```
=等于
>大于
<小于
>=大于等于
<=小于等于
<>不等于
```
```sql
select * from info where score in(100, 88);//满足100跟88
select * from info where score between 80 and 90;
```
②like
```sql
select * from info where name like 'w%';
select * from info where name like 'tian__';
```
### 更新数据
```sql
update info set age=19 where num=3;
update info set age=18 where between 80 and 90;
update info set age=23, score=130.0 where name='tianqi';
```
### 删除表格
```sql
drop table 表格名称;
```
### 新增字段
```sql
alter table 表名 add 字段名 数据类型;
```
【注意：目前sqlite不支持删除已有字段】
 
### 更新表名
```sql
alter table 原表名 rename to 新名;
```
练习：
--创建雇员表
```sql
CREATE TABLE EMPLOYEES(
employee_id Integer PRIMARY KEY,
department_id Integer,
location_id Integer,
first_name,
last_name,
salary,
hire_date date
);
```
--创建部门表
```sql
CREATE TABLE DEPARTMENT(department_id Integer primary key,name);
```
--创建职位表
```sql
```
--添加测试数据
```sql
insert into employees(department_id,location_id , first_name, last_name, salary,hire_date)
values (1,1, 'A', 'z', 50000, '2005-02-21');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (1,2, 'B', 'x', 20000, '2009-03-21');
insert into employees(department_id,location_id , first_name, last_name, salary,hire_date)
values (2,3, 'C', 'v', 10000, '2009-08-23');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (4,2, 'D', 'n', 30000, '2004-09-28');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (3,5, 'E', 'm', 3000, '2009-04-11');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,5, 'F', 'l', 5000, '2008-03-11');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,3, 'G', 'p', 20000, '2005-05-09');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,4, 'H', 'o', 8000, '2006-07-21');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,5, 'I', 'u', 6000, '2006-09-21');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,5, 'J', 'y', 5500, '2007-08-21');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,5, 'K', 't', 6500, '2006-12-21');
insert into employees(department_id,location_id, first_name, last_name, salary,hire_date)
values (5,1, 'L', 'r', 100000, '2001-05-21'); 

insert into department(name) values ('人事');
insert into department(name) values ('财务');
insert into department(name) values ('后勤');
insert into department(name) values ('公关');
insert into department(name) values ('研发');

insert into location(name) values ('总经理');
insert into location(name) values ('经理');
insert into location(name) values ('主管');
insert into location(name) values ('组长');
insert into location(name) values ('职员');
```
--查研发部的职员的员工信息
```sql
select * from employees e where e.location_id =(select l.location_id from location l 
where l.name='职员')
and e.[department_id]=(select d.department_id from department d where d.name='研发');
```
--根据查询结果创建一个表
```sql
create table TEMP_EMPLOYEES AS select employee_id, first_name, last_name from 
EMPLOYEES where salary>6000;
```
--查询可以进行计算
```sql
select salary*13 年薪 from employees where 年薪 !=260000;
select salary*13 年薪 from employees where 年薪 between 50000 and 100000;
```
--first_name 是A, B, C的职员信息
```sql
select * from employees where first_name in ('A', 'B', 'C');
```
--测试is null
```sql
select * from employees where first_name is null;
```
--查询月薪大于10000的主管
```sql
select * from employees where salary>10000 and location_id=3;
```
--查询月薪大于10000的主管或者经理
```sql
select * from employees where salary>10000 and (location_id=3 or location_id=2);
select * from employees where salary>10000 and location_id not in(4,5,1);
```
--测试order by
```sql
select * from employees order by salary;
select * from employees order by hire_date;
```
sqlite可视化工具:SQLite Studio
SQLite数据库可视化工具【SQLiteStudio】的下载及安装-CSDN博客

练习：使用sqlite3QSL语句，创建一个公司人员记录表格。有员工：工号，姓名，年龄，薪水，部门（必须分配到已有部门）。
尝试按照薪水输出所有员工信息，或者自己实现一些查找操作，比如查找薪水高于10000的员工。并列出来。
## 视图
### 创建视图
创建视图：
```sql
CREATE VIEW view-name AS select-statement
CREATE VIEW view1 AS select * from info2 where score between 90 and 100;
create view empinfo AS select * from employees e where e.location_id =(select l.location_id from location l 
where l.name='职员')
and e.department_id=(select d.department_id from department d where d.name='研发');
```
### 查看视图
```shell
.tables
```
### 查看视图详情
```sql
select * from empinfo;
```
### 删除视图
```sql
drop view empinfo;
```
## SQlite给C和C++编程接口
https://www.sqlite.org/cintro.html

头文件：`#include <sqlite3.h>`
### 打开数据库
```c++
int sqlite3_open(const char *filename, //数据库文件名
sqlite3 **ppDb);//数据库句柄指针
```

 
### 执行sql命令
```c++
int sqlite3_exec(sqlite3 *db, //已经打开的数据库句柄
const char *sql, //想要执行的sql命令
int (*callback)(void*,int,char**,char**), //回调函数
void *p, //给回调函数传入的参数
char **errmsg);// 错误信息
 
//回调函数的定义：
这个回调函数随着查询过程每找到一个满足要求的数据记录就会调用一遍
int func(void *data, int column, char **value, char **cname)
void *data:执行sqlite3_exec函数的时候传入的参数
int column:查询到的每个数据记录有多少个字段的信息
char **value:每个字段的内容
char **cname:每个字段的名字
```
 
### 关闭数据库
```c++
int sqlite3_close(sqlite3*);
```
-------
## 编译
（1）使用sqlite3的源码进行编译  
①先将sqlite3.c和sqlite3.h拷贝到源码路径下   
②与主函数代码一起编译  
gcc stu.c sqlite3.c -o stu -lpthread -ldl
 
（2）使用sqlite3的库进行编译  
①先安装支持库：  
sudo apt-get install libsqlite3-dev  
②直接编译：   
gcc stu.c -o stu -lsqlite3

 
## 练习：
建一个学生信息数据库：  
做一张学生成绩表：学号、班级、成绩。数据初始插入二十条。  
做一张学生信息表：学号、姓名、班级、年龄、电话号码  
自定义回调函数，实现查询命令的数据返回显示。  
（1）查询并显示所有班级为1的学生基本信息；  
select * from stuinfo where class = 1;
（2）查询并显示年龄超过20岁的学生人数；  
select count(*) from stuinfo where age > 20;
（3）查询并显示2班所有成绩低于60分的学生的姓名和电话号码。   
select name,tel from stuinfo s where
s.num in (select t.num from scoretable t where 
t.class = 2 and t.score < 60);
