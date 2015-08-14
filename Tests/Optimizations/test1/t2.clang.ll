; ModuleID = 't2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %v = alloca i32*, align 8
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call noalias i8* @malloc(i64 24) #3
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %v, align 8
  %1 = load i32* %argc.addr, align 4
  %2 = load i32** %v, align 8
  %arrayidx = getelementptr inbounds i32* %2, i64 0
  store i32 %1, i32* %arrayidx, align 4
  %3 = load i32* %argc.addr, align 4
  %4 = load i32** %v, align 8
  %arrayidx1 = getelementptr inbounds i32* %4, i64 1
  store i32 %3, i32* %arrayidx1, align 4
  %5 = load i32* %argc.addr, align 4
  %rem = srem i32 %5, 3
  switch i32 %rem, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb2
  ]

sw.bb:                                            ; preds = %entry
  store i32 0, i32* %a, align 4
  store i32 3, i32* %b, align 4
  br label %sw.epilog

sw.bb2:                                           ; preds = %entry
  store i32 1, i32* %a, align 4
  store i32 4, i32* %b, align 4
  br label %sw.epilog

sw.default:                                       ; preds = %entry
  store i32 2, i32* %a, align 4
  store i32 5, i32* %b, align 4
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb2, %sw.bb
  %6 = load i32* %b, align 4
  %idxprom = sext i32 %6 to i64
  %7 = load i32** %v, align 8
  %arrayidx3 = getelementptr inbounds i32* %7, i64 %idxprom
  %8 = load i32* %arrayidx3, align 4
  %9 = load i32* %a, align 4
  %idxprom4 = sext i32 %9 to i64
  %10 = load i32** %v, align 8
  %arrayidx5 = getelementptr inbounds i32* %10, i64 %idxprom4
  %11 = load i32* %arrayidx5, align 4
  %add = add nsw i32 %11, %8
  store i32 %add, i32* %arrayidx5, align 4
  %12 = load i32* %b, align 4
  %idxprom6 = sext i32 %12 to i64
  %13 = load i32** %v, align 8
  %arrayidx7 = getelementptr inbounds i32* %13, i64 %idxprom6
  %14 = load i32* %arrayidx7, align 4
  %15 = load i32* %a, align 4
  %idxprom8 = sext i32 %15 to i64
  %16 = load i32** %v, align 8
  %arrayidx9 = getelementptr inbounds i32* %16, i64 %idxprom8
  %17 = load i32* %arrayidx9, align 4
  %add10 = add nsw i32 %17, %14
  store i32 %add10, i32* %arrayidx9, align 4
  %18 = load i32** %v, align 8
  %19 = load i32** %v, align 8
  %add.ptr = getelementptr inbounds i32* %19, i64 1
  %call11 = call i32 @no_body_sum(i32* %18, i32* %add.ptr)
  %20 = load i32* %retval
  ret i32 %20
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
