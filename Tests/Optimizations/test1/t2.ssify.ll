; ModuleID = 't2.ssify.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %call = call noalias i8* @malloc(i64 24) #3
  %tmp = bitcast i8* %call to i32*
  %arrayidx = getelementptr inbounds i32* %tmp, i64 0
  store i32 %argc, i32* %arrayidx, align 4
  %arrayidx1 = getelementptr inbounds i32* %tmp, i64 1
  store i32 %argc, i32* %arrayidx1, align 4
  %rem = srem i32 %argc, 3
  switch i32 %rem, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb2
  ]

sw.bb:                                            ; preds = %entry
  br label %sw.epilog

sw.bb2:                                           ; preds = %entry
  br label %sw.epilog

sw.default:                                       ; preds = %entry
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb2, %sw.bb
  %a.0 = phi i32 [ 2, %sw.default ], [ 1, %sw.bb2 ], [ 0, %sw.bb ]
  %b.0 = phi i32 [ 5, %sw.default ], [ 4, %sw.bb2 ], [ 3, %sw.bb ]
  %idxprom = sext i32 %b.0 to i64
  %arrayidx3 = getelementptr inbounds i32* %tmp, i64 %idxprom
  %tmp1 = load i32* %arrayidx3, align 4
  %idxprom4 = sext i32 %a.0 to i64
  %arrayidx5 = getelementptr inbounds i32* %tmp, i64 %idxprom4
  %tmp2 = load i32* %arrayidx5, align 4
  %add = add nsw i32 %tmp2, %tmp1
  store i32 %add, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %b.0 to i64
  %arrayidx7 = getelementptr inbounds i32* %tmp, i64 %idxprom6
  %tmp3 = load i32* %arrayidx7, align 4
  %idxprom8 = sext i32 %a.0 to i64
  %arrayidx9 = getelementptr inbounds i32* %tmp, i64 %idxprom8
  %tmp4 = load i32* %arrayidx9, align 4
  %add10 = add nsw i32 %tmp4, %tmp3
  store i32 %add10, i32* %arrayidx9, align 4
  %add.ptr = getelementptr inbounds i32* %tmp, i64 1
  %call11 = call i32 @no_body_sum(i32* %tmp, i32* %add.ptr)
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
