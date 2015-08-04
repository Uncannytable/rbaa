; ModuleID = 't.ssify.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nounwind uwtable
define void @foo(i32* %v) #0 {
entry:
  %idxprom = sext i32 0 to i64
  %arrayidx = getelementptr inbounds i32* %v, i64 %idxprom
  store i32 10, i32* %arrayidx, align 4
  %idxprom1 = sext i32 1 to i64
  %arrayidx2 = getelementptr inbounds i32* %v, i64 %idxprom1
  store i32 20, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 0 to i64
  %arrayidx4 = getelementptr inbounds i32* %v, i64 %idxprom3
  %tmp = load i32* %arrayidx4, align 4
  %call = call i32 (i8*, i32, ...)* bitcast (i32 (...)* @prinf to i32 (i8*, i32, ...)*)(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %tmp)
  ret void
}

declare i32 @prinf(...) #1

; Function Attrs: nounwind uwtable
define void @bar(i32* %v, i32 %a, i32 %b) #0 {
entry:
  %idxprom = sext i32 %a to i64
  %arrayidx = getelementptr inbounds i32* %v, i64 %idxprom
  store i32 10, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %b to i64
  %arrayidx2 = getelementptr inbounds i32* %v, i64 %idxprom1
  store i32 20, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 %a to i64
  %arrayidx4 = getelementptr inbounds i32* %v, i64 %idxprom3
  %tmp = load i32* %arrayidx4, align 4
  %call = call i32 (i8*, i32, ...)* bitcast (i32 (...)* @prinf to i32 (i8*, i32, ...)*)(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %call = call noalias i8* @malloc(i64 8) #3
  %tmp = ptrtoint i8* %call to i32
  %conv = sext i32 %tmp to i64
  %tmp1 = inttoptr i64 %conv to i32*
  call void @foo(i32* %tmp1)
  %conv1 = sext i32 %tmp to i64
  %tmp2 = inttoptr i64 %conv1 to i32*
  call void @bar(i32* %tmp2, i32 0, i32 1)
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
