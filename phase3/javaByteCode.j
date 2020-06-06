.source program.txt
.class public result
.super java/lang/Object

.method public <init>()V
aload_0
invokenonvirtual java/lang/Object/<init>()V
return
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 100
.limit stack 100
iconst_0
istore 1
fconst_0
fstore 2
.line 1
iconst_0
istore 3
.line 2
L_0:
iconst_0
istore 4
.line 3
L_1:
ldc 5
istore 3
.line 4
L_2:
iload 3
ldc 5
if_icmpeq L_3
goto L_10
.line 5
L_3:
.line 6
ldc 100
istore 3
.line 7
L_4:
iconst_0
istore 5
.line 8
L_5:
ldc 9
istore 5
.line 9
L_6:
L_7:
iload 5
ldc 0
if_icmpge L_8
goto L_11
L_8:
.line 10
iload 5
istore 1
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 11
L_9:
iload 5
ldc 1
isub
istore 5
.line 12
goto L_7
.line 13
goto L_11
.line 14
.line 15
L_10:
.line 16
ldc 9
istore 3
.line 17
.line 18
L_11:
iload 3
istore 1
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 19
L_12:
return
.end method
