# MyBatis

- MyBatis是一款优秀的持久层框架,用于简化JDBC开发
- MyBatis本是Apache的一个开源项目iBatis,2010年这个项目由apache software foundation 迁移到了google xode,并改名MyBatis.2013年迁移到Github
- [官网](https://mybatis.org/mybatis-3/zh/index.html)

*持久层*
- 负责将数据保存到数据库的那一层代码
- JavaEE三层架构:表现层,业务层,持久层

*框架*
- 框架就是一个半成品软件,是一套可重用,通用的,软件基础代码模型
- 在框架的基础之上构建软件编写更加高效,规范,通用,可扩展

JDBC缺点
1. 硬编码
    - 注册驱动,获取连接
    - SQL语句
2. 操作繁琐
    - 手动设置参数
    - 手动封装结果集

解决方法
1. 将硬编码部分写道一个配置文件中
2. 操作繁琐就让它自动完成
结果
- MyBatis免除了几乎所有的JDBC代码以及设置参数和获取结果集的工作

其他持久层同类型框架

    MyBatis-Plus
    Spring Data JPA
    Hibernate
    ...

## MyBatis开发大致过程
1. 创建表
2. 创建模块,导入坐标
3. 编写MyBatis核心配置文件 -- > 替换连接信息解决硬编码问题
4. 编写SQL映射文件 -->通用管理SQL语句
5. 编码
   1. 定义PoJo类
   2. 加载核心配置文件,获取SqlSessionFactory对象
   3. 获取SqlSession对象,执行SQL语句
   4. 释放资源 
*如果出现SQL语句爆红,则是IDEA未连接数据库*
## Mapper代理开发
1.  定义与SQL映射文件同名的Mapper接口,并将Mapper接口和SQL映射文件放在同一目录下
2.  设置SQL映射文件的namespace属性为Mapper接口全限定名
3.  在Mapper接口中定义方法,方法名就是SQL映射文件中sql语句的id,并保持参数类型和返回值类型一致
4.  编码
    1.  通过调用SqlSession的getMapper方法获取Mapper接口的代理对象
    2.  调用对应方法完成sql的执行

*注:当Mapper接口名称和SQL映射文件名称相同,并在同一目录下,则可以使用包扫描的方式简化SQL映射文件的加载*
```xml
<mapper>
    <!--如果在同一目录就可以省略-->
    <!--<mapper resource='(xml配置文件地址)'>-->
    <package name='(包名)'/>
</mapper>
```
## MyBatis核心配置文件
*[官网查看](https://mybatis.org/mybatis-3/zh/configuration.html)*

* configuration（配置）
  * properties（属性）
  * settings（设置）
  * typeAliases（类型别名）
  * typeHandlers（类型处理器）
  * objectFactory（对象工厂）
  * plugins（插件）
  * environments（环境配置）
    * environment（环境变量）
      * transactionManager（事务管理器）
      * dataSource（数据源）
  * databaseIdProvider（数据库厂商标识）
  * mappers（映射器）  

类型别名(包)
```xml
<typeAliases>
    <!--写好以后就不用带包名-->
    <package name="包名"/>
</typeAliases>
```    


*配置各个标签时注意前后的顺序*
## 配置文件完成增删改查


## 注解完成增删改查


## 动态SQL

## MybatisX插件
一块基于IDEA的快速开发插件,为效率而生
* XML和接口方法相互跳转
* 根据接口方法生成statement