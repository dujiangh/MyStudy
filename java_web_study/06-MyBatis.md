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
## MyBatis参数传递

Mybatis接口方法中可以接受各种各样的参数,Mybatis地层对于这些参数采用不同的封装处理方式,不建议使用默认的名称,如arg0或parm1;推荐使用@parm注解替换Map集合中默认arg键名

- 单个参数
    1. pojo类型   直接使用,属性名和参数占位符名称一致
    2. Map集合  直接使用,键名和参数占位符名称一致
    3. Collection  封装成Map集合,两个键对应的集合类型,arg0和collectoin
    4. List  三个键对应的集合类型,arg0,collectoin和list
    5. Array 封装成Map集合,arg0和array
    6. 其他类型:可以直接使用
- 多个参数
  - 会封装成Map集合来创建参数,需要一个键和一个值,一个参数会产生两个键,  带Param注解时会替换Map中arg的键名
    - map.put("arg0",参数值1)
    - map.put("param1",参数值1)
    - map.put("arg1",参数值2)
    - map.put("param2",参数值2)
    ```java
    User select(@Param("usename") String username,@Param("password") String password)
    //也可以不写注解,那么变量名就得用arg0,param1
    User select(String username,String password)
    ```
    ```xml
    <select id="select" resultType="user">
    select *
    from tb_user
    where
    username=#{username}
    and password=#{password}
    </select>
    <!-- 当不写注解时,但不推荐 -->
    <select id="select" resultType="user">
    select *
    from tb_user
    where
    username=#{arg0}
    and password=#{param2}
    </select>
    ```
MyBatis提供了ParamNameResolver类来进行参数封装  
建议:  
将来都建议使用@parm注解修改map中的默认键名,并使用修改后的名称来获取值,这样可读性更高    
单个参数中的Collection,List,Array和其他类型,还有多个参数使用注解  
## 配置文件完成增删改查


## 注解完成增删改查
注解完成简单功能  
配置文件完成复杂功能  

*用注解来映射简单语句会使代码显得更加简洁,但对于稍微复杂一点的浯句 Java 注解不仅力不从心,还会让你本就复杂 SQL 语句更加混乱不堪.因此,如果你需要做一些很复杂的操作,最好用 XML 来映射语句*  

*选择何种方式来配置映射,以及认为是否应该统一映射语句定义的形式,完全取决于你和你团队换句话说,永远不要拘泥于一种方式,你可以很轻松的在基于注解和XML的语句映射方式间自由移植和切换*
## 动态SQL

## MybatisX插件
一块基于IDEA的快速开发插件,为效率而生
* XML和接口方法相互跳转
* 根据接口方法生成statement