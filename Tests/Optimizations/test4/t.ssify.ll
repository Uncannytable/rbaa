; ModuleID = 't.ssify.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nounwind uwtable
define void @foo() #0 {
entry:
  %call = call noalias i8* @malloc(i64 12) #3
  %tmp = bitcast i8* %call to i32*
  %add.ptr = getelementptr inbounds i32* %tmp, i64 1
  %add.ptr1 = getelementptr inbounds i32* %tmp, i64 2
  store i32 10, i32* %add.ptr, align 4
  store i32 20, i32* %add.ptr1, align 4
  %tmp1 = load i32* %add.ptr, align 4
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %tmp1)
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: nounwind uwtable
define void @bar(i32* %a, i32* %b) #0 {
entry:
  store i32 10, i32* %a, align 4
  store i32 20, i32* %b, align 4
  %tmp = load i32* %a, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  call void @foo()
  %call = call noalias i8* @malloc(i64 12) #3
  %tmp = bitcast i8* %call to i32*
  %add.ptr = getelementptr inbounds i32* %tmp, i64 1
  %add.ptr1 = getelementptr inbounds i32* %tmp, i64 2
  call void @bar(i32* %add.ptr, i32* %add.ptr1)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
