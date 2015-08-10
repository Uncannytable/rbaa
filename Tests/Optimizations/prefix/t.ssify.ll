; ModuleID = 't.ssify.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %mul = mul nsw i32 %argc, 2
  %conv = sext i32 %mul to i64
  %call = call noalias i8* @malloc(i64 %conv) #2
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc15, %for.inc14 ]
  %cmp = icmp slt i32 %i.0, %argc
  br i1 %cmp, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  %SSIfy_sigma = phi i32 [ %i.0, %for.cond ]
  %idxprom = sext i32 %SSIfy_sigma to i64
  %arrayidx = getelementptr inbounds i8* %call, i64 %idxprom
  store i8 0, i8* %arrayidx, align 1
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp3 = icmp slt i32 %j.0, %argc
  br i1 %cmp3, label %for.body5, label %for.end

for.body5:                                        ; preds = %for.cond2
  %SSIfy_sigma3 = phi i32 [ %j.0, %for.cond2 ]
  %add = add nsw i32 %SSIfy_sigma3, %argc
  %idxprom6 = sext i32 %add to i64
  %arrayidx7 = getelementptr inbounds i8* %call, i64 %idxprom6
  %tmp = load i8* %arrayidx7, align 1
  %conv8 = sext i8 %tmp to i32
  %idxprom9 = sext i32 %SSIfy_sigma to i64
  %arrayidx10 = getelementptr inbounds i8* %call, i64 %idxprom9
  %tmp1 = load i8* %arrayidx10, align 1
  %conv11 = sext i8 %tmp1 to i32
  %add12 = add nsw i32 %conv11, %conv8
  %conv13 = trunc i32 %add12 to i8
  store i8 %conv13, i8* %arrayidx10, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body5
  %inc = add nsw i32 %SSIfy_sigma3, 1
  br label %for.cond2

for.end:                                          ; preds = %for.cond2
  %SSIfy_sigma4 = phi i32 [ %j.0, %for.cond2 ]
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %inc15 = add nsw i32 %SSIfy_sigma, 1
  br label %for.cond

for.end16:                                        ; preds = %for.cond
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
