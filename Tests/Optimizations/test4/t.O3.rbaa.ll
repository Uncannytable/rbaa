; ModuleID = 't.O3.rbaa.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nounwind uwtable
define void @foo() #0 {
entry:
  %call = tail call noalias i8* @malloc(i64 12) #2
  %add.ptr = getelementptr inbounds i8* %call, i64 4
  %0 = bitcast i8* %add.ptr to i32*
  %add.ptr1 = getelementptr inbounds i8* %call, i64 8
  %1 = bitcast i8* %add.ptr1 to i32*
  store i32 10, i32* %0, align 4
  store i32 20, i32* %1, align 4
  %tmp1 = load i32* %0, align 4
  %call2 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0), i32 %tmp1) #2
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #1

; Function Attrs: nounwind uwtable
define void @bar(i32* nocapture %a, i32* nocapture %b) #0 {
entry:
  store i32 10, i32* %a, align 4
  store i32 20, i32* %b, align 4
  %tmp = load i32* %a, align 4
  %call = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0), i32 %tmp) #2
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
entry:
  %call.i = tail call noalias i8* @malloc(i64 12) #2
  %add.ptr.i = getelementptr inbounds i8* %call.i, i64 4
  %0 = bitcast i8* %add.ptr.i to i32*
  %add.ptr1.i = getelementptr inbounds i8* %call.i, i64 8
  %1 = bitcast i8* %add.ptr1.i to i32*
  store i32 10, i32* %0, align 4
  store i32 20, i32* %1, align 4
  %tmp1.i = load i32* %0, align 4
  %call2.i = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0), i32 %tmp1.i) #2
  %call = tail call noalias i8* @malloc(i64 12) #2
  %add.ptr = getelementptr inbounds i8* %call, i64 4
  %2 = bitcast i8* %add.ptr to i32*
  %add.ptr1 = getelementptr inbounds i8* %call, i64 8
  %3 = bitcast i8* %add.ptr1 to i32*
  store i32 10, i32* %2, align 4
  store i32 20, i32* %3, align 4
  %tmp.i = load i32* %2, align 4
  %call.i1 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0), i32 %tmp.i) #2
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
