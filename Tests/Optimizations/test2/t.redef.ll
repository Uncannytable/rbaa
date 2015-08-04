; ModuleID = 't.redef.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %C = alloca [2 x i8], align 1
  %A = alloca [10 x i8], align 1
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp ne i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %SSIfy_sigma = phi i32 [ %i.0, %for.cond ]
  %idxprom = sext i32 %SSIfy_sigma to i64
  %arrayidx = getelementptr inbounds [10 x i8]* %A, i32 0, i64 %idxprom
  %tmp = load i8* %arrayidx, align 1
  %arrayidx1 = getelementptr inbounds [2 x i8]* %C, i32 0, i64 0
  store i8 %tmp, i8* %arrayidx1, align 1
  %sub = sub nsw i32 9, %SSIfy_sigma
  %idxprom2 = sext i32 %sub to i64
  %arrayidx3 = getelementptr inbounds [10 x i8]* %A, i32 0, i64 %idxprom2
  %tmp1 = load i8* %arrayidx3, align 1
  %arrayidx4 = getelementptr inbounds [2 x i8]* %C, i32 0, i64 1
  store i8 %tmp1, i8* %arrayidx4, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %SSIfy_sigma, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
