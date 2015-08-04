; ModuleID = 't.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %C = alloca [2 x i8], align 1
  %A = alloca [10 x i8], align 1
  store i32 0, i32* %retval
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32* %i, align 4
  %cmp = icmp ne i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32* %i, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [10 x i8]* %A, i32 0, i64 %idxprom
  %2 = load i8* %arrayidx, align 1
  %arrayidx1 = getelementptr inbounds [2 x i8]* %C, i32 0, i64 0
  store i8 %2, i8* %arrayidx1, align 1
  %3 = load i32* %i, align 4
  %sub = sub nsw i32 9, %3
  %idxprom2 = sext i32 %sub to i64
  %arrayidx3 = getelementptr inbounds [10 x i8]* %A, i32 0, i64 %idxprom2
  %4 = load i8* %arrayidx3, align 1
  %arrayidx4 = getelementptr inbounds [2 x i8]* %C, i32 0, i64 1
  store i8 %4, i8* %arrayidx4, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %6 = load i32* %retval
  ret i32 %6
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
