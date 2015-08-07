; ModuleID = 't.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nounwind uwtable
define void @foo() #0 {
entry:
  %v = alloca i32*, align 8
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %call = call noalias i8* @malloc(i64 12) #3
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %v, align 8
  %1 = load i32** %v, align 8
  %add.ptr = getelementptr inbounds i32* %1, i64 1
  store i32* %add.ptr, i32** %a, align 8
  %2 = load i32** %v, align 8
  %add.ptr1 = getelementptr inbounds i32* %2, i64 2
  store i32* %add.ptr1, i32** %b, align 8
  %3 = load i32** %a, align 8
  store i32 10, i32* %3, align 4
  %4 = load i32** %b, align 8
  store i32 20, i32* %4, align 4
  %5 = load i32** %a, align 8
  %6 = load i32* %5, align 4
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %6)
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: nounwind uwtable
define void @bar(i32* %a, i32* %b) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  store i32* %a, i32** %a.addr, align 8
  store i32* %b, i32** %b.addr, align 8
  %0 = load i32** %a.addr, align 8
  store i32 10, i32* %0, align 4
  %1 = load i32** %b.addr, align 8
  store i32 20, i32* %1, align 4
  %2 = load i32** %a.addr, align 8
  %3 = load i32* %2, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %3)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %v = alloca i32*, align 8
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  call void @foo()
  %call = call noalias i8* @malloc(i64 12) #3
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %v, align 8
  %1 = load i32** %v, align 8
  %add.ptr = getelementptr inbounds i32* %1, i64 1
  store i32* %add.ptr, i32** %a, align 8
  %2 = load i32** %v, align 8
  %add.ptr1 = getelementptr inbounds i32* %2, i64 2
  store i32* %add.ptr1, i32** %b, align 8
  %3 = load i32** %a, align 8
  %4 = load i32** %b, align 8
  call void @bar(i32* %3, i32* %4)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
