; ModuleID = 'demo.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, i8** noundef %1) #0 !dbg !8 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !16, metadata !DIExpression()), !dbg !17
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !18, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.declare(metadata i32* %6, metadata !20, metadata !DIExpression()), !dbg !21
  store i32 0, i32* %6, align 4, !dbg !21
  call void @llvm.dbg.declare(metadata i32* %7, metadata !22, metadata !DIExpression()), !dbg !23
  %9 = load i32, i32* %4, align 4, !dbg !24
  store i32 %9, i32* %7, align 4, !dbg !23
  call void @llvm.dbg.declare(metadata i32* %8, metadata !25, metadata !DIExpression()), !dbg !26
  %10 = load i32, i32* %7, align 4, !dbg !27
  store i32 %10, i32* %8, align 4, !dbg !26
  %11 = load i32, i32* %6, align 4, !dbg !28
  %12 = load i32, i32* %7, align 4, !dbg !29
  %13 = add nsw i32 %11, %12, !dbg !30
  store i32 %13, i32* %8, align 4, !dbg !31
  %14 = load i32, i32* %6, align 4, !dbg !32
  ret i32 %14, !dbg !33
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.ident = !{!2}
!llvm.module.flags = !{!3, !4, !5, !6, !7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 14.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "source/main.c", directory: "/home/cs6890/Program-Analysis/pa_template/benchmark/bench4", checksumkind: CSK_MD5, checksum: "1783fcdd29e69c125e075a5b435c86cf")
!2 = !{!"clang version 14.0.6"}
!3 = !{i32 7, !"Dwarf Version", i32 5}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"uwtable", i32 1}
!7 = !{i32 7, !"frame-pointer", i32 2}
!8 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 3, type: !9, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !15)
!9 = !DISubroutineType(types: !10)
!10 = !{!11, !11, !12}
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!15 = !{}
!16 = !DILocalVariable(name: "argc", arg: 1, scope: !8, file: !1, line: 3, type: !11)
!17 = !DILocation(line: 3, column: 14, scope: !8)
!18 = !DILocalVariable(name: "argv", arg: 2, scope: !8, file: !1, line: 3, type: !12)
!19 = !DILocation(line: 3, column: 26, scope: !8)
!20 = !DILocalVariable(name: "z", scope: !8, file: !1, line: 5, type: !11)
!21 = !DILocation(line: 5, column: 9, scope: !8)
!22 = !DILocalVariable(name: "x", scope: !8, file: !1, line: 6, type: !11)
!23 = !DILocation(line: 6, column: 9, scope: !8)
!24 = !DILocation(line: 6, column: 13, scope: !8)
!25 = !DILocalVariable(name: "y", scope: !8, file: !1, line: 7, type: !11)
!26 = !DILocation(line: 7, column: 9, scope: !8)
!27 = !DILocation(line: 7, column: 13, scope: !8)
!28 = !DILocation(line: 8, column: 9, scope: !8)
!29 = !DILocation(line: 8, column: 13, scope: !8)
!30 = !DILocation(line: 8, column: 11, scope: !8)
!31 = !DILocation(line: 8, column: 7, scope: !8)
!32 = !DILocation(line: 10, column: 12, scope: !8)
!33 = !DILocation(line: 10, column: 5, scope: !8)
