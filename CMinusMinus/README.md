# C Minus Minus (C--)

## 一、 編譯器支援的文法規則與功能
- 以下為文法規則
```
    Globalstatments -> Globalstatment Globalstatments | ε
    Globalstatment -> FunctionDefinition

    DeclarationExpression -> VariableType DeclarationVariableList
    DeclarationStatement -> DeclarationExpression ;
    VariableType -> Int | Void
    DeclarationVariableList -> DeclarationVariable | DeclarationVariableList , DeclarationVariable
    DeclarationVariable -> Identifier | Identifier ASSIGN Expression

    AssignmentExpression -> Identifier = Expression
    AssignmentStatement -> AssignmentExpression ;

    Statements -> Statement Statements | ε
    Statement -> DeclarationStatement | AssignmentStatement | CallFunctionStatement | IfStatement | WhileStatement | ForStatement | PrintStatement | ε

    FunctionDeclarationOrDefinition -> VariableType Identifier ( Parameters ) FunctionBlock | VariableType Identifier ( Parameters )
    Parameters -> ε | ParameterList
    ParameterList -> Parameter | ParameterList , Parameter
    Parameter -> VariableType Identifier
    FunctionBlock -> { FunctionBlockStatements }
    FunctionBlockStatements -> FunctionBlockStatement FunctionBlockStatements | ε
    FunctionBlockStatement -> Statement | ReturnStatement
    ReturnStatement -> retrun ; | return Expression ;

    LoopBlockStatements -> LoopBlockStatement LoopBlockStatements | ε
    LoopBlockStatement -> Statement | ReturnStatement | FlowBreakStatement
    FlowBreakStatement -> break ; | continue ;

    Block -> { FunctionBlockStatements } | FunctionBlockStatement
    LoopBlock -> { LoopBlockStatements } | LoopBlockStatement

    ForInitExpression -> AssignmentExpression | DeclarationExpression

    IfStatement -> if ( ConditionExpression ) LoopBlock | if ( ConditionExpression ) LoopBlock else LoopBlock
    WhileStatement -> while ( ConditionExpression ) LoopBlock
    ForStatement -> for ( ForInitExpression ; ConditionExpression ; AssignmentExpression ) LoopBlock
    PrintStatemnt -> print ( Expression ) ;

    CallFunctionExpression -> Identifier ( CallFunctionParameters )
    CallFunctionStatement -> CallFunctionExpression ;
    CallFunctionParameters -> CallFunctionParameters | Expression | ε

    ConditionExpression -> Expression < Expression | Expression > Expression | Expression <= Expression | Expression >= Expression | Expression == Expression | Expression != Expression

    Expression -> Term | Expression + Term | Expression - Term
    Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    Factor -> IntValue | Identifier | CallFunctionExpression
```
- 大略描述支援的語法
1. 函數聲明
2. 函數定義
3. 變數創建
4. 更改變數資訊
5. 使用print(變數名稱)將變數資訊印出
6. if else if else
7. while
8. for
9. 支援迴圈內continue與break
10. 函數呼叫
11. 單行與多行註解

- 支援數值類型
1. void
2. int

## 二、 編譯器製作的過程：如何撰寫、遭遇的困難、如何解決問題

### 1. 如何撰寫
本程式完全基於C++進行撰寫，從Lexer到Parser到最後的生成中間形式程式碼皆由本人撰寫\

#### Lexer:
1. 將關鍵字與特殊符號進行辨識，並放到對應的Token中
2. 將數字類型放到int類型當中
3. 其他不符合規範的文字會直接報錯
```
Code: 
1. CMinusMinus/Lexer/lexer.h
2. CMinusMinus/Lexer/lexer.cpp
```

#### Parser:
根據上述的文法規則進行parse，本parser為一次性解析，在解析的過程中就會直接生成中間形式程式碼，所以不會有生成語法樹的過程
```
Code:
1. CMinusMinus/Parser/parser.h
2. CMinusMinus/Parser/parser.cpp
```

#### LLVM IR
本次使用LLVM IR做為中間形式程式碼，故會在parser的時候不斷呼叫LLVMController來生成對應的LLVM IR程式碼\
以下是使用LLVM IR做為中間形式程式碼的優點
```
LLVM IR (Intermediate Representation) 是 LLVM 編譯器基礎設施的核心部分，具有多種優點，使其成為強大且靈活的中間表示語言。以下是一些主要的優點：

1. **平台無關性**：
   - LLVM IR 是與目標硬體無關的中間表示，這意味著它可以在不同的硬體平台上生成和優化代碼。這樣，開發者只需編寫一次代碼，就可以針對多種不同的硬體架構進行編譯。

2. **豐富的優化工具**：
   - LLVM 提供了豐富的優化工具和技術，可以對 LLVM IR 進行各種優化，包括常量折疊、死代碼消除、循環優化等，從而產生更高效的機器代碼。

3. **豐富的語言支持**：
   - 除了 C 和 C++，LLVM 還支持眾多其他語言，如 Swift、Rust、Haskell、Kotlin 等，這使得它成為一個多語言後端編譯器。

4. **可移植性和可重用性**：
   - 由於 LLVM IR 是一種中間表示語言，不同的前端編譯器可以生成相同的 LLVM IR，而後端可以將這些 IR 轉換為特定目標機器碼。這大大提高了代碼的可移植性和可重用性。
```
```
Code:
1. CMinusMinus/LLVMController/llvm_controller.h
2. CMinusMinus/LLVMController/llvm_controller.cpp
```

### 2. 遭遇的困難
1. 對編譯器不熟悉，同時也不清楚目前開發編譯器推薦的方法
2. 教學資源相對較少

### 3. 如何解決問題
1. 在尚未開始製作前上網調查目前推薦的開發工具與方法
    - 發現目前主流語言皆透過LLVM IR作為中間形式程式碼
    - 研究LLVM IR的優點與如何使用
    - 確定使用LLVM IR作為中間形式程式碼
2. 解決教學資源較少
    - 先找到最基礎的教學，了解最基礎用法
    - 使用GPT與Copilot輔助撰寫

## 三、編譯器製作的成果：測試程式執行結果的截圖

### 測試程式
測試程式放在[CMinusMinus/sample_code.txt]當中，可以直接過去查看
```c
int sum(int a, int b);
void doSomething();

int fab(int n) {
    if (n <= 1) {
        return 1;
    }
    return fab(n - 1) + fab(n - 2);
}

int main() {
    int a = 20 + 10 * 2 / 5, b;
    b = a + 20 / 2;
    int c = a;
    print(c);
    c = 10 + 20 + 33 % 5;
    print(10 + 20 + 33);
    print(c);
    c = sum(sum(10, 20), 20);
    print(c);
    if (1 < 2) {
        print(1);
        if (sum(10, 20) == 30) {
            print(30);
        }
    }
    int idx = 0;
    while (idx < 2) {
        idx = idx + 1;
        print(idx);
        int index = 0;
        while (index < 2) {
            index = index + 1;
            print(index);
        }
    }
    for (int nxt = 0; nxt < 5; nxt = nxt + 1) {
        print(nxt);
        for (int nxtt = 10; nxtt < 12; nxtt = nxtt + 1) {
            print(nxtt);
        }
    }
    for (int f = 0; f < 10; f = f + 1) {
        if (f % 2 == 0) {
            continue;
        }
        if (f >= 6) {
            break;
        }
        print(f);
    }
    print(fab(10));
    return 0;
}

int sum(int a, int b) {
    return a + b;
}

void doSomething() {
    int idx = 100;
    while (idx < 10) {
        return;
        print(idx);
    }
    for (int a = 0; a < 2; a = a + 1) {
        return;
        print(a);
    }
    if (idx < 10) {
        print(idx);
        if (idx > 0) {
            return;
            print(idx);
        }
        print(idx);
    }
    return;
}
```
### 執行結果
```
24
63
33
50
1
30
1
1
2
2
1
2
0
10
11
1
10
11
2
10
11
3
10
11
4
10
11
1
3
5
89
```

## 四、編譯器製作的心得
這次製作編譯器讓我拿回許久沒有寫的C++，重新體驗C++的美好，同時也讓自己進入一個學習新東西的狀態，對於任何小小的成功都會有大大的成就感，最重要的是大致了解線下最流行的編譯器製作方法。\
不過還是有些可惜的地方，像是最終我還是只有支援int類型的資料，因為暫時不太確定到底要怎麼在parse的過程中確定當前expression到底最後是int還是float。\
最後還是需要感謝生在這個有AI輔助的時代，雖然他無法完全協助完成作業，但是可以有效的提供大致的框架，不須面對完全空白的畫面，導致無法開始撰寫。

## 五、參考文獻
1. LLVM IR 簡易教學: https://b23.tv/C7hGpUA
2. 手嚕編譯器: https://b23.tv/C7hGpUA

## 六、如何使用

### 製作編譯器
```
1. 安裝LLVM並完成相關環境設定
2. 進入CMinusCMinus資料夾
3. 使用以下指令
clang++ -I/opt/homebrew/opt/llvm/include -L/opt/homebrew/opt/llvm/lib -lLLVM main.cpp ./Lexer/lexer.cpp ./Parser/parser.cpp ./LLVMController/llvm_controller.cpp -o a.out --std=c++17

以上指令是要編譯[main.cpp, lexer.cpp, parser.cpp, llvm_controller.cpp]
我的環境是在mac上，如果環境有所不同就自行查詢編譯方法，總之就是要將上述的檔案進行編譯並生成出執行檔
```
### 編譯CMinusMinus
1. 準備一個檔案A.txt
2. 在A.txt當中撰寫你的程式(須符合上述的文法規範)
3. 使用以下指令
```
./a.out < A.txt
llvm-as a.ll -o a.bc
llc a.bc -o a.s
gcc a.s -o run.out
```
```
Code:
1. CMinusMinus/build.sh
```

4. 指令說明
    - ./a.out < A.txt : 將A.txt給a.out進行編譯，會生成出a.ll檔案
        - a.ll: 這個就是LLVM IR程式碼，可以看到與組合語言有點類似
    - llvm-as a.ll -o a.bc: 將a.ll透過LLVM生成出二進制檔案
    - llc a.bc -o a.s: 將a.bc透過llc根據當前平台轉換出對應的組合語言
    - gcc a.s -o run.out: 最終A.txt的執行檔
```
Code:
1. CMinusMinus/pipline.sh
# 註: pipline.sh會先執行build.sh
```

### Note
由於本次作業是在macOS上完成的所以最終提供的sample_code執行檔會是mac可執行的，如果是其他平台的就須自行編譯

## 七、其他
1. 判斷是否有main function
    - 如果發現沒有main function只會給出警告，但是依舊可以正常的編譯
2. 原始程式碼連結
    - https://github.com/chris901003/SimpleCCompiler/tree/master/CMinusMinus