Makefile所有内嵌函数
一、文本处理函数
以下是GNU make内嵌的文本（字符串）处理函数。
1       $(subst FROM,TO,TEXT)

函数名称：字符串替换函数—subst。

函数功能：把字串“TEXT”中的“FROM”字符替换为“TO”。

返回值：替换后的新字符串。

示例：

$(subst ee,EE,feet on the street)

替换“feet on the street”中的“ee”为“EE”，结果得到字符串“fEEt on the strEEt”。
2       $(patsubst PATTERN,REPLACEMENT,TEXT)

函数名称：模式替换函数—patsubst。

函数功能：搜索“TEXT”中以空格分开的单词，将否符合模式“TATTERN”替换为“REPLACEMENT”。参数“PATTERN”中可以使用模式通配符“%”来代表一个单词中的若干字符。如果参数“REPLACEMENT”中也包含一个“%”，那么“REPLACEMENT”中的“%”将是“TATTERN”中的那个“%”所代表的字符串。在“TATTERN”和“REPLACEMENT”中，只有第一个“%”被作为模式字符来处理，之后出现的不再作模式字符（作为一个字符）。在参数中如果需要将第一个出现的“%”作为字符本身而不作为模式字符时，可使用反斜杠“\”进行转义处理。

返回值：替换后的新字符串。

函数说明：参数“TEXT”单词之间的多个空格在处理时被合并为一个空格，并忽略前导和结尾空格。

示例：

$(patsubst %.c,%.o,x.c.c bar.c)

  把字串“x.c.c bar.c”中以.c结尾的单词替换成以.o结尾的字符。函数的返回结果是“x.c.o bar.o”

  变量的替换引用，它是一个简化版的“patsubst”函数在变量引用过程的实现。变量替换引用中：

$(VAR:PATTERN=REPLACEMENT)

  就相当于：

$(patsubst PATTERN,REPLACEMENT,$(VAR))

 

而另外一种更为简单的替换字符后缀的实现：

$(VAR:SUFFIX=REPLACEMENT)

它等于：

$(patsubst %SUFFIX,%REPLACEMENT,$(VAR))

  例如我们存在一个代表所有.o文件的变量。定义为“objects = foo.o bar.o baz.o”。为了得到这些.o文件所对应的.c源文件。我们可以使用以下两种方式的任意一个：

$(objects:.o=.c)

$(patsubst %.o,%.c,$(objects))
patsubst三个操作数用逗号隔开
作用：将foo的所有由空格隔开的字符串尾的.c用.o代替
出错提示：Makefile:4: *** insufficient number of arguments (2) to function `patsubst'.  Stop.
格式举例：bar := $(patsubst %.c,%.o,$(foo))
3       $(strip STRINT)

函数名称：去空格函数—strip。

函数功能：去掉字串（若干单词，使用若干空字符分割）“STRINT”开头和结尾的空字符，并将其中多个连续空字符合并为一个空字符。

返回值：无前导和结尾空字符、使用单一空格分割的多单词字符串。

函数说明：空字符包括空格、[Tab]等不可显示字符。

示例：

STR =        a    b c    

LOSTR = $(strip $(STR))

结果是“a b c”。

“strip”函数经常用在条件判断语句的表达式中，确保表达式比较的可靠和健壮！
4       $(findstring FIND,IN)

函数名称：查找字符串函数—findstring。

函数功能：搜索字串“IN”，查找“FIND”字串。

返回值：如果在“IN”之中存在“FIND”，则返回“FIND”，否则返回空。

函数说明：字串“IN”之中可以包含空格、[Tab]。搜索需要是严格的文本匹配。

示例：

$(findstring a,a b c)

$(findstring a,b c)

第一个函数结果是字“a”；第二个值为空字符。
5       $(filter PATTERN…,TEXT)

函数名称：过滤函数—filter。

函数功能：过滤掉字串“TEXT”中所有不符合模式“PATTERN”的单词，保留所有符合此模式的单词。可以使用多个模式。模式中一般需要包含模式字符“%”。存在多个模式时，模式表达式之间使用空格分割。

返回值：空格分割的“TEXT”字串中所有符合模式“PATTERN”的字串。

函数说明：“filter”函数可以用来去除一个变量中的某些字符串，我们下边的例子中就是用到了此函数。

示例：

sources := foo.c bar.c baz.s ugh.h

foo: $(sources)

cc $(filter %.c %.s,$(sources)) -o foo

  使用“$(filter %.c %.s,$(sources))”的返回值给cc来编译生成目标“foo”，函数返回值为“foo.c bar.c baz.s”。
6       $(filter-out PATTERN...,TEXT)

函数名称：反过滤函数—filter-out。

函数功能：和“filter”函数实现的功能相反。过滤掉字串“TEXT”中所有符合模式“PATTERN”的单词，保留所有不符合此模式的单词。可以有多个模式。存在多个模式时，模式表达式之间使用空格分割。。

返回值：空格分割的“TEXT”字串中所有不符合模式“PATTERN”的字串。

函数说明：“filter-out”函数也可以用来去除一个变量中的某些字符串，（实现和“filter”函数相反）。

示例：

objects=main1.o foo.o main2.o bar.o

mains=main1.o main2.o

 

$(filter-out $(mains),$(objects))

实现了去除变量“objects”中“mains”定义的字串（文件名）功能。它的返回值为“foo.o bar.o”。
7       $(sort LIST)

函数名称：排序函数—sort。

函数功能：给字串“LIST”中的单词以首字母为准进行排序（升序），并去掉重复的单词。

返回值：空格分割的没有重复单词的字串。

函数说明：两个功能，排序和去字串中的重复单词。可以单独使用其中一个功能。

示例：

$(sort foo bar lose foo)

  返回值为：“bar foo lose” 。
8       $(word N,TEXT)

函数名称：取单词函数—word。

函数功能：取字串“TEXT”中第“N”个单词（“N”的值从1开始）。

返回值：返回字串“TEXT”中第“N”个单词。

函数说明：如果“N”值大于字串“TEXT”中单词的数目，返回空字符串。如果“N”为0，出错！

示例：

$(word 2, foo bar baz)

返回值为“bar”。
9       $(wordlist S,E,TEXT)

函数名称：取字串函数—wordlist。

函数功能：从字串“TEXT”中取出从“S”开始到“E”的单词串。“S”和“E”表示单词在字串中位置的数字。

返回值：字串“TEXT”中从第“S”到“E”（包括“E”）的单词字串。

函数说明：“S”和“E”都是从1开始的数字。

当“S”比“TEXT”中的字数大时，返回空。如果“E”大于“TEXT”字数，返回从“S”开始，到“TEXT”结束的单词串。如果“S”大于“E”，返回空。

示例：

$(wordlist 2, 3, foo bar baz)

返回值为：“bar baz”。
10  $(words TEXT)

函数名称：统计单词数目函数—words。

函数功能：字算字串“TEXT”中单词的数目。

返回值：“TEXT”字串中的单词数。

示例：

$(words, foo bar)

返回值是“2”。所以字串“TEXT”的最后一个单词就是：$(word $(words TEXT),TEXT)。
11  $(firstword NAMES…)

函数名称：取首单词函数—firstword。

函数功能：取字串“NAMES…”中的第一个单词。

返回值：字串“NAMES…”的第一个单词。

函数说明：“NAMES”被认为是使用空格分割的多个单词（名字）的序列。函数忽略“NAMES…”中除第一个单词以外的所有的单词。

示例：

$(firstword foo bar)

返回值为“foo”。函数“firstword”实现的功能等效于“$(word 1, NAMES…)”。

 

以上11个函数是make内嵌的的文本处理函数。书写Makefile时可搭配使用来实现复杂功能。最后我们使用这些函数来实现一个实际应用。例子中我们使用函数“subst”和“patsbust”。Makefile中可以使用变量“VPATH”来指定搜索路径。对于源代码所包含的头文件的搜索路径需要使用gcc的“-I”参数指定目录来实现，“VPATH”罗列的目录是用冒号“:”分割的。如下就是Makefile的片段：

……

VPATH = src:../includes

override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))

…….

那么第二条语句所实现的功能就是“CFLAGS += -Isrc –I../includes”。

二、文件名处理函数

GNU make除支持文本处理函数之外，还支持一些针对于文件名的处理函数。这些函数主要用来对一系列空格分割的文件名进行转换，这些函数的参数被作为若干个文件名来对待。函数对作为参数的一组文件名按照一定方式进行处理并返回空格分割的多个文件名序列。
1       $(dir NAMES…)

函数名称：取目录函数—dir。

函数功能：从文件名序列“NAMES…”中取出各个文件名的目录部分。文件名的目录部分就是包含在文件名中的最后一个斜线（“/”）（包括斜线）之前的部分。

返回值：空格分割的文件名序列“NAMES…”中每一个文件的目录部分。

函数说明：如果文件名中没有斜线，认为此文件为当前目录（“./”）下的文件。

示例：

$(dir src/foo.c hacks)

返回值为“src/ ./”。
2       $(notdir NAMES…)

函数名称：取文件名函数——notdir。

函数功能：从文件名序列“NAMES…”中取出非目录部分。目录部分是指最后一个斜线（“/”）（包括斜线）之前的部分。删除所有文件名中的目录部分，只保留非目录部分。

返回值：文件名序列“NAMES…”中每一个文件的非目录部分。

函数说明：如果“NAMES…”中存在不包含斜线的文件名，则不改变这个文件名。以反斜线结尾的文件名，是用空串代替，因此当“NAMES…”中存在多个这样的文件名时，返回结果中分割各个文件名的空格数目将不确定！这是此函数的一个缺陷。

示例：

$(notdir src/foo.c hacks)

  返回值为：“foo.c hacks”。
3       $(suffix NAMES…)

函数名称：取后缀函数—suffix。

函数功能：从文件名序列“NAMES…”中取出各个文件名的后缀。后缀是文件名中最后一个以点“.”开始的（包含点号）部分，如果文件名中不包含一个点号，则为空。

返回值：以空格分割的文件名序列“NAMES…”中每一个文件的后缀序列。

函数说明：“NAMES…”是多个文件名时，返回值是多个以空格分割的单词序列。如果文件名没有后缀部分，则返回空。

示例：

$(suffix src/foo.c src-1.0/bar.c hacks)

返回值为“.c .c”。
4       $(basename NAMES…)

函数名称：取前缀函数—basename。

函数功能：从文件名序列“NAMES…”中取出各个文件名的前缀部分（点号之后的部分）。前缀部分指的是文件名中最后一个点号之前的部分。

返回值：空格分割的文件名序列“NAMES…”中各个文件的前缀序列。如果文件没有前缀，则返回空字串。

函数说明：如果“NAMES…”中包含没有后缀的文件名，此文件名不改变。如果一个文件名中存在多个点号，则返回值为此文件名的最后一个点号之前的文件名部分。

示例：

$(basename src/foo.c src-1.0/bar.c /home/jack/.font.cache-1 hacks)

  返回值为：“src/foo src-1.0/bar /home/jack/.font hacks”。
5       $(addsuffix SUFFIX,NAMES…)

函数名称：加后缀函数—addsuffix。

函数功能：为“NAMES…”中的每一个文件名添加后缀“SUFFIX”。参数“NAMES…”为空格分割的文件名序列，将“SUFFIX”追加到此序列的每一个文件名的末尾。

返回值：以单空格分割的添加了后缀“SUFFIX”的文件名序列。

函数说明：

示例：

$(addsuffix .c,foo bar)

返回值为“foo.c bar.c”。
6       $(addprefix PREFIX,NAMES…)

函数名称：加前缀函数—addprefix。

函数功能：为“NAMES…”中的每一个文件名添加前缀“PREFIX”。参数“NAMES…”是空格分割的文件名序列，将“SUFFIX”添加到此序列的每一个文件名之前。

返回值：以单空格分割的添加了前缀“PREFIX”的文件名序列。

函数说明：

示例： 

$(addprefix src/,foo bar) 

返回值为“src/foo src/bar”。
7       $(join LIST1,LIST2)

函数名称：单词连接函数——join。

函数功能：将字串“LIST1”和字串“LIST2”各单词进行对应连接。就是将“LIST2”中的第一个单词追加“LIST1”第一个单词字后合并为一个单词；将“LIST2”中的第二个单词追加到“LIST1”的第一个单词之后并合并为一个单词，……依次列推。

返回值：单空格分割的合并后的字（文件名）序列。

函数说明：如果“LIST1”和“LIST2”中的字数目不一致时，两者中多余部分将被作为返回序列的一部分。

示例1：

$(join a b , .c .o)

返回值为：“a.c b.o”。

示例2：

$(join a b c , .c .o)

返回值为：“a.c b.o c”。
8       $(wildcard PATTERN)

函数名称：获取匹配模式文件名函数—wildcard

函数功能：列出当前目录下所有符合模式“PATTERN”格式的文件名。

返回值：空格分割的、存在当前目录下的所有符合模式“PATTERN”的文件名。

函数说明：“PATTERN”使用shell可识别的通配符，包括“?”（单字符）、“*”（多字符）等。

示例：

$(wildcard *.c)

返回值为当前目录下所有.c源文件列表。

三、foreach函数

函数“foreach”不同于其它函数。它是一个循环函数。类似于Linux的shell中的for语句。

“foreach”函数的语法：

  $(foreach VAR,LIST,TEXT)

  函数功能：这个函数的工作过程是这样的：如果需要（存在变量或者函数的引用），首先展开变量“VAR”和“LIST”的引用；而表达式“TEXT”中的变量引用不展开。执行时把“LIST”中使用空格分割的单词依次取出赋值给变量“VAR”，然后执行“TEXT”表达式。重复直到“LIST”的最后一个单词（为空时结束）。“TEXT”中的变量或者函数引用在执行时才被展开，因此如果在“TEXT”中存在对“VAR”的引用，那么“VAR”的值在每一次展开式将会到的不同的值。

返回值：空格分割的多次表达式“TEXT”的计算的结果。

看一个例子，定义变量“files”，它的值为四个目录（变量“dirs”代表的a、b、c、d四个目录）下的文件列表：

  dirs := a b c d

files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))

  例子中，“TEXT”的表达式为“$(wildcard $(dir)/*)”。表达式第一次执行时将展开为“$(wildcard a/*)”；第二次执行时将展开为“$(wildcard b/*)”；第三次展开为“$(wildcard c/*)”；….；以此类推。所以此函数所实现的功能就和一下语句等价：

  files := $(wildcard a/* b/* c/* d/*)

  当函数的“TEXT”表达式过于复杂时，我们可以通过定义一个中间变量，此变量代表表达式的一部分。并在函数的“TEXT”中引用这个变量。上边的例子也可以这样来实现：

  find_files = $(wildcard $(dir)/*)

dirs := a b c d

files := $(foreach dir,$(dirs),$(find_files))

在这里我们定义了一个变量（也可以称之为表达式），需要注意，在这里定义的是“递归展开”时的变量“find_files”。保证了定义时变量值中的引用不展开，而是在表达式被函数处理时才展开（如果这里使用直接展开式的定义将是无效的表达式）。

  函数说明：函数中参数“VAR”是一个局部的临时变量，它只在“foreach”函数的上下文中有效，它的定义不会影响其它部分定义的同名“VAR”变量的值。在函数的执行过程中它是一个“直接展开”式变量。

 

在使用函数“foreach”时，需要注意：变量“VAR”的名字。我们建议使用一个单词、最好能够表达其含义的名字，不要使用一个奇怪的字符串作为变量名。虽然执行是不会发生错误，但是会让人很费解。

没有人会喜欢这种方式，尽管可能它可以正常工作：

  files := $(foreach Esta escrito en espanol!,b c ch,$(find_files))
四、if函数

函数“if”提供了一个在函数上下文中实现条件判断的功能。就像make所支持的条件语句—ifeq一样。

  函数语法：

$(if CONDITION,THEN-PART[,ELSE-PART])

  函数功能：第一个参数“CONDITION”，在函数执行时忽略其前导和结尾空字符，如果包含对其他变量或者函数的引用则进行展开。如果“CONDITION”的展开结果非空，则条件为真，就将第二个参数“THEN_PATR”作为函数的计算表达式；“CONDITION”的展开结果为空，将第三个参数“ELSE-PART”作为函数的表达式，函数的返回结果为有效表达式的计算结果。

   返回值：根据条件决定函数的返回值是第一个或者第二个参数表达式的计算结果。当不存在第三个参数“ELSE-PART”，并且“CONDITION”展开为空，函数返回空。

  函数说明：函数的条件表达式“CONDITION”决定了函数的返回值只能是“THEN-PART”或者“ELSE-PART”两个之一的计算结果。

   函数示例：

  SUBDIR += $(if $(SRC_DIR) $(SRC_DIR),/home/src)

  函数的结果是：如果“SRC_DIR”变量值不为空，则将变量“SRC_DIR”指定的目录作为一个子目录；否则将目录“/home/src”作为一个子目录。

五、call函数

“call”函数是唯一一个可以创建定制化参数函数的引用函数。使用这个函数可以实现对用户自己定义函数引用。我们可以将一个变量定义为一个复杂的表达式，用“call”函数根据不同的参数对它进行展开来获得不同的结果。

函数语法：

  $(call VARIABLE,PARAM,PARAM,...)

  函数功能：在执行时，将它的参数“PARAM”依次赋值给临时变量“$(1)”、“$(2)”（这些临时变量定义在“VARIABLE”的值中，参考下边的例子）…… call函数对参数的数目没有限制，也可以没有参数值，没有参数值的“call”没有任何实际存在的意义。执行时变量“VARIABLE”被展开为在函数上下文有效的临时变量，变量定义中的“$(1)”作为第一个参数，并将函数参数值中的第一个参数赋值给它；变量中的“$(2)”一样被赋值为函数的第二个参数值；依此类推（变量$(0)代表变量“VARIABLE”本身）。之后对变量“VARIABLE” 表达式的计算值。

返回值：参数值“PARAM”依次替换“$(1)”、“$(2)”…… 之后变量“VARIABLE”定义的表达式的计算值。（使用call函数“VARIABLE”中一定存在“$(1)”、“$(2)”…… ）

函数说明：1. 函数中“VARIBLE”是一个变量名，而不是变量引用。因此，通常“call”函数中的“VARIABLE”中不包含“$”（当然，除非此变量名是一个计算的变量名）。2. 当变量“VARIBLE”是一个make内嵌的函数名时（如“if”、“foreach”、“strip”等），对“PARAM”参数的使用需要注意，因为不合适或者不正确的参数将会导致函数的返回值难以预料。3. 函数中多个“PARAM”之间使用逗号分割。4. 变量“VARIABLE”在定义时不能定义为直接展开式！只能定义为递归展开式。

函数示例：

首先，来看一个简单的例子。

示例1：

  reverse =  $(2) $(1)

foo = $(call reverse,a,b)

  变量“foo”的值为“ba”。这里变量“reverse”中的参数定义顺序可以根据需要来调整，并不是需要按照“$(1)”、“$(2)”、“$(3)”…… 这样的顺序来定义。


看一个稍微复杂一些的例子。我们定义了一个宏“pathsearch”来在“PATH”路径中搜索第一个指定的程序。

示例2：

  pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))

LS := $(call pathsearch,ls)

  变量“LS”的结果为“/bin/sh”。执行过程：函数“subst”将环境变量“PATH”转换为空格分割的搜索路径列表；“addsuffix”构造出可能的可执行程序“$(1)”（这里是“ls”）带路径的完整文件名（如：“/bin/$(1)”），之后使用函数“wildcard”匹配，最后“firstword”函数取第一个文件名。

 

函数“call”以可以套嵌使用。每一层“call”函数的调用都为它自己的局部变量“$(1)”等赋值，覆盖上一层函数为它所赋的值。

示例3：

map = $(foreach a,$(2),$(call $(1),$(a)))

o = $(call map,origin,o map MAKE)

 

那么变量“o”的值就为“file file default”。我们这里使用了“origin”函数。我们分析函数的执行过程：首先，“o=$(call map,origin, o map MAKE)”这个函数调用使用了变量“map”所定义的表达式；使用内嵌函数名“origin”作为它的第一个参数值，使用Makefile中的变量“o map MAKE”作为他的第二个参数值。当使用“call”函数展开后等价于“$(foreach a,o map MAKE,$(origin $(a)))”。

注意：和其它函数一样，“call”函数会保留出现在其参数值列表中的空字符。因此在使用参数值时对空格处理要格外小心。如果参数中存在多余的空格，函数可能会返回一个莫名奇妙的值。为了安全，在变量作为“call”函数参数值之前，应去掉其值中的多余空格。


六、value函数

函数“value”提供了一种在不对变量进行展开的情况下获取变量值的方法。注意：并不是说函数会取消之前已经执行过的替换扩展。比如：定义了一个直接展开式的变量，此变量在定义过程中对其它变量的引用进行替换而得到自身的值。在使用“value”函数取这个变量进行取值时，得到的是不包含任何引用值。而不是将定义过程中的替换展开动作取消后包含引用的定义值。就是说此过程不能取消此变量在定义时已经发生了的替换展开动作。

函数语法：

$(value VARIABLE)

 

函数功能：不对变量“VARIBLE”进行任何展开操作，直接返回变量“VARIBALE”的值。这里“VARIABLE”是一个变量名，一般不包含“$”（除非计算的变量名），

返回值：变量“VARIBALE”所定义文本值（如果变量定义为递归展开式，其中包含对其他变量或者函数的引用，那么函数不对这些引用进行展开。函数的返回值是包含有引用值）。

函数说明：

示例：

# sample Makefile

FOO = $PATH

 

all:

@echo $(FOO)

@echo $(value FOO)

执行make，可以看到的结果是：第一行为：“ATH”。这是因为变量“FOO”定义为“$PATH”，所以展开为“ATH”（“$P”为空）。

第二行才是我们需要显示的系统环境变量“PATH”的值（value函数得到变量“FOO”的值为“$PATH”）。


七、eval函数

函数功能：函数“eval”是一个比较特殊的函数。使用它可以在Makefile中构造一个可变的规则结构关系（依赖关系链），其中可以使用其它变量和函数。函数“eval”对它的参数进行展开，展开的结果作为Makefile的一部分，make可以对展开内容进行语法解析。展开的结果可以包含一个新变量、目标、隐含规则或者是明确规则等。也就是说此函数的功能主要是：根据其参数的关系、结构，对它们进行替换展开。

返回值：函数“eval”的返回值时空，也可以说没有返回值。

函数说明：“eval”函数执行时会对它的参数进行两次展开。第一次展开过程是由函数本身完成的，第二次是函数展开后的结果被作为Makefile内容时由make解析时展开的。明确这一过程对于使用“eval”函数非常重要。理解了函数“eval”二次展开的过程后。实际使用时，如果在函数的展开结果中存在引用（格式为：$(x)），那么在函数的参数中应该使用“$$”来代替“$”。因为这一点，所以通常它的参数中会使用函数“value”来取一个变量的文本值。

我们看一个例子。例子看起来似乎非常复杂，因为它综合了其它的一些概念和函数。不过我们可以考虑两点：其一，通常实际一个模板的定义可能比例子中的更为复杂；其二，我们可以实现一个复杂通用的模板，在所有Makefile中包含它，亦可作到一劳永逸。相信这一点可能是大多数程序员所推崇的。

示例：

# sample Makefile 

PROGRAMS    = server client

    

server_OBJS = server.o server_priv.o server_access.o

server_LIBS = priv protocol

    

client_OBJS = client.o client_api.o client_mem.o

client_LIBS = protocol

    

# Everything after this is generic

.PHONY: all

all: $(PROGRAMS)

    

define PROGRAM_template

$(1): $$($(1)_OBJ) $$($(1)_LIBS:%=-l%)

ALL_OBJS   += $$($(1)_OBJS)

endef

    

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

    

$(PROGRAMS):

$(LINK.o) $^ $(LDLIBS) -o $@

    

clean:

rm -f $(ALL_OBJS) $(PROGRAMS)

 

来看一下这个例子：它实现的功能是完成“PROGRAMS”的编译链接。例子中“$(LINK.o)”为“$(CC) $(LDFLAGS)”，意思是对所有的.o文件和指定的库文件进行链接。

“$(foreach prog,$(PROGRAM),$(eval $(call PROGRAM_template,$(prog))))”展开为：

server : $(server_OBJS) –l$(server_LIBS)

client : $(client_OBJS) –l$(client_LIBS)


八、origin函数

函数“origin”和其他函数不同，函数“origin”的动作不是操作变量（它的参数）。它只是获取此变量（参数）相关的信息，告诉我们这个变量的出处（定义方式）。

函数语法：

$(origin VARIABLE)

 

函数功能：函数“origin”查询参数“VARIABLE”（一个变量名）的出处。

函数说明：“VARIABLE”是一个变量名而不是一个变量的引用。因此通常它不包含“$”（当然，计算的变量名例外）。

返回值：返回“VARIABLE”的定义方式。用字符串表示。

函数的返回情况有以下几种：

1.        undefined

变量“VARIABLE”没有被定义。

2.        default

变量“VARIABLE”是一个默认定义（内嵌变量）。如“CC”、“MAKE”、“RM”等变量。如果在Makefile中重新定义这些变量，函数返回值将相应发生变化。

3.        environment

变量“VARIABLE”是一个系统环境变量，并且make没有使用命令行选项“-e”（Makefile中不存在同名的变量定义，此变量没有被替代）。

4.        environment override

变量“VARIABLE”是一个系统环境变量，并且make使用了命令行选项“-e”。Makefile中存在一个同名的变量定义，使用“make -e”时环境变量值替代了文件中的变量定义。

5.        file

变量“VARIABLE”在某一个makefile文件中定义。

6.        command line

变量“VARIABLE”在命令行中定义。

7.        override

变量“VARIABLE”在makefile文件中定义并使用“override”指示符声明。

8.        automatic

变量“VARIABLE”是自动化变量。

 

函数“origin”返回的变量信息对我们书写Makefile是相当有用的，可以使我们在使用一个变量之前对它值的合法性进行判断。假设在Makefile其包了另外一个名为bar.mk的makefile文件。我们需要在bar.mk中定义变量“bletch”（无论它是否是一个环境变量），保证“make –f bar.mk”能够正确执行。另外一种情况，当Makefile包含bar.mk，在Makefile包含bar.mk之前有同样的变量定义，但是我们不希望覆盖bar.mk中的“bletch”的定义。一种方式是：我们在bar.mk中使用指示符“override”声明这个变量。但是它所存在的问题时，此变量不能被任何方式定义的同名变量覆盖，包括命令行定义。另外一种比较灵活的实现就是在bar.mk中使用“origin”函数，如下：

ifdef bletch

ifeq "$(origin bletch)" "environment"

bletch = barf, gag, etc.

endif

endif

这里，如果存在环境变量“bletch”，则对它进行重定义。

ifneq "$(findstring environment,$(origin bletch))" ""

bletch = barf, gag, etc.

endif

这个例子实现了：即使环境变量中已经存在变量“bletch”，无论是否使用“make -e”来执行Makefile，变量“bletch”的值都是“barf,gag,etc”（在Makefile中所定义的）。环境变量不能替代文件中的定义。

如果“$(origin bletch)”返回“environment”或“environment override”，都将对变量“bletch”重新定义。


九、shell函数

shell函数不同于除“wildcard”函数之外的其它函数。make可以使用它来和外部通信。

函数功能：函数“shell”所实现的功能和shell中的引用（``）相同。实现对命令的扩展。这就意味着需要一个shell 命令作为此函数的参数，函数的返回结果是此命令在shell中的执行结果。make仅仅对它的回返结果进行处理；make将函数返回结果中的所有换行符（“\n”）或者一对“\n\r”替换为单空格；并去掉末尾的回车符号（“\n”）或者“\n\r”。进行函数展开式时，它所调用的命令（它的参数）得到执行。除对它的引用出现在规则的命令行和递归变量的定义中以外，其它决大多数情况下，make是在读取解析Makefile时完成对函数shell的展开。

返回值：函数“shell”的参数（一个shell命令）在shell环境中的执行结果。

函数说明：函数本身的返回值是其参数的执行结果，没有进行任何处理。对结果的处理是由make进行的。当对函数的引用出现在规则的命令行中，命令行在执行时函数才被展开。展开时函数参数（shell命令）的执行是在另外一个shell进程中完成的，因此需要对出现在规则命令行的多级“shell”函数引用需要谨慎处理，否则会影响效率（每一级的“shell”函数的参数都会有各自的shell进程）。

示例1：

contents := $(shell cat foo)

将变量“contents”赋值为文件“foo”的内容，文件中的换行符在变量中使用空格代替。

示例2：

files := $(shell echo *.c)

将变量“files”赋值为当前目录下所有.c文件的列表（文件名之间使用空格分割）。在shell中之行的命令是“echo *.c”，此命令返回当前目录下的所有.c文件列表。上例的执行结果和函数“$(wildcard *.c)”的结果相同，除非你使用的是一个奇怪的shell。
注意：通过上边的两个例子我们可以看到，当引用“shell”函数的变量定义使用直接展开式定义时可以保证函数的展开是在make读入Makefile时完成。后续对此变量的引用就不会有展开过程。这样就可以避免规则命令行中的变量引用在命令行执行时展开的情况发生（因为展开“shell”函数需要另外的shell进程完成，影响命令的执行效率）。这也是我们建议的方式。

十、make的控制函数

make提供了两个控制make运行方式的函数。通常它们用在Makefile中，当make执行过程中检测到某些错误是为用户提供消息，并且可以控制make过程是否继续。
1  $(error TEXT…)

函数功能：产生致命错误，并提示“TEXT…”信息给用户，并退出make的执行。需要说明的是：“error”函数是在函数展开式（函数被调用时）才提示信息并结束make进程。因此如果函数出现在命令中或者一个递归的变量定义中时，在读取Makefile时不会出现错误。而只有包含“error”函数引用的命令被执行，或者定义中引用此函数的递归变量被展开时，才会提示致命信息“TEXT…”同时退出make。

返回值：空

  函数说明：“error”函数一般不出现在直接展开式的变量定义中，否则在make读取Makefile时将会提示致命错误。

假设我们的Makefile中包含以下两个片断；

示例1：
ifdef ERROR1
$(error error is $(ERROR1))
endif 

make读取解析Makefile时，如果只起那已经定义变量“EROOR1”，make将会提示致命错误信息“$(ERROR1)”并退出。

示例2：

ERR = $(error found an error!)

 

.PHONY: err

err: ; $(ERR) 

这个例子，在make读取Makefile时不会出现致命错误。只有目标“err”被作为一个目标被执行时才会出现。
2  $(warning TEXT…) 

函数功能：函数“warning”类似于函数“error”，区别在于它不会导致致命错误（make不退出），而只是提示“TEXT…”，make的执行过程继续。

返回值：空

函数说明：用法和“error”类似，展开过程相同。