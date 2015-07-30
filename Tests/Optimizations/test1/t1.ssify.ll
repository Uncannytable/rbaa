; ModuleID = 't1.ssify.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %call = call noalias i8* @malloc(i64 24) #3
  %tmp = bitcast i8* %call to i32*
  %arrayidx = getelementptr inbounds i32* %tmp, i64 0
  store i32 13, i32* %arrayidx, align 4
  %arrayidx1 = getelementptr inbounds i32* %tmp, i64 1
  store i32 17, i32* %arrayidx1, align 4
  %idxprom = sext i32 1 to i64
  %arrayidx2 = getelementptr inbounds i32* %tmp, i64 %idxprom
  %tmp1 = load i32* %arrayidx2, align 4
  %idxprom3 = sext i32 0 to i64
  %arrayidx4 = getelementptr inbounds i32* %tmp, i64 %idxprom3
  %tmp2 = load i32* %arrayidx4, align 4
  %add = add nsw i32 %tmp2, %tmp1
  store i32 %add, i32* %arrayidx4, align 4
  %idxprom5 = sext i32 1 to i64
  %arrayidx6 = getelementptr inbounds i32* %tmp, i64 %idxprom5
  %tmp3 = load i32* %arrayidx6, align 4
  %idxprom7 = sext i32 0 to i64
  %arrayidx8 = getelementptr inbounds i32* %tmp, i64 %idxprom7
  %tmp4 = load i32* %arrayidx8, align 4
  %add9 = add nsw i32 %tmp4, %tmp3
  store i32 %add9, i32* %arrayidx8, align 4
  %add.ptr = getelementptr inbounds i32* %tmp, i64 1
  %call10 = call i32 @no_body_sum(i32* %tmp, i32* %add.ptr)
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i32 @no_body_sum(i32*, i32*) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
