; ModuleID = 'b01.redef.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @bench_A(i32* %a) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %SSIfy_sigma = phi i32 [ %i.0, %for.cond ]
  %add = add nsw i32 %SSIfy_sigma, 2
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds i32* %a, i64 %idxprom
  %tmp = load i32* %arrayidx, align 4
  %add1 = add nsw i32 %SSIfy_sigma, 1
  %idxprom2 = sext i32 %add1 to i64
  %arrayidx3 = getelementptr inbounds i32* %a, i64 %idxprom2
  store i32 %tmp, i32* %arrayidx3, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %SSIfy_sigma, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %call = call noalias i8* @malloc(i64 400) #2
  %tmp = ptrtoint i8* %call to i32
  %conv = sext i32 %tmp to i64
  %tmp1 = inttoptr i64 %conv to i32*
  call void @bench_A(i32* %tmp1)
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
