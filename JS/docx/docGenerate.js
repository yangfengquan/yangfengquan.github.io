function doc_generate(title, form, result) {
    console.log(result);
    let rows = [];
    let head_row = new docx.TableRow({
        children: [
            new docx.TableCell({
                children: [
                    new docx.Paragraph({
                        style: "tableHead",
                        text: "名称",
                    })
                ],
                width: {
                    size: 40,
                    type: docx.WidthType.PERCENTAGE,
                },
            }),
            new docx.TableCell({
                children: [
                    new docx.Paragraph({
                        style: "tableHead",
                        text: "数值",
                    })
                ],
                width: {
                    size: 40,
                    type: docx.WidthType.PERCENTAGE,
                },
            }),
            new docx.TableCell({
                children: [
                    new docx.Paragraph({
                        style: "tableHead",
                        text: "单位",
                    })
                ],
                width: {
                    size: 40,
                    type: docx.WidthType.PERCENTAGE,
                },
            }),
        ],
        tableHeader: true,
    });


    rows.push(head_row);
    for (let i = 0; i < form.length; i++) {
        let row = new docx.TableRow({
            children: [
            new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: form[i].name,
                        })
                    ],
                    width: {
                        size: 40,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
                new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: form[i].value,
                        })
                    ],
                    width: {
                        size: 40,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
                new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: form[i].unit,
                        })
                    ],
                    width: {
                        size: 20,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
            ],
        });
        rows.push(row);
    }
    const table_form = new docx.Table({
        rows: rows,
        width: {
            size: 8760,
            type: docx.WidthType.DXA,
        }
    });

    rows = [];
    rows.push(head_row);
    for (let i = 0; i < result.length; i++) {
        let row = new docx.TableRow({
            children: [
                new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: result[i].name,
                        })
                    ],
                    width: {
                        size: 40,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
                new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: result[i].value.toString(),
                        })
                    ],
                    width: {
                        size: 40,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
                new docx.TableCell({
                    children: [
                        new docx.Paragraph({
                            style: "listParagraph",
                            text: result[i].unit,
                        })
                    ],
                    width: {
                        size: 20,
                        type: docx.WidthType.PERCENTAGE,
                    },
                }),
            ],
        });
        rows.push(row);
    }
    const table_result = new docx.Table({
        rows: rows,
        width: {
            size: 8760,
            type: docx.WidthType.DXA,
        }
    });
    const doc = new docx.Document({
        creator: window.location.hostname,
        title: "Report",
        description: "yangshu.gitee.io",
        styles: {
            paragraphStyles: [
                {
                    id: "Heading1",
                    name: "Heading 1",
                    basedOn: "Normal",
                    next: "Normal",
                    quickFormat: true,
                    run: {
                        size: 28,
                        bold: true,
                        font: "黑体"
                        //italics: true,
                    },
                    paragraph: {
                        spacing: {
                            before: 240,
                            after: 120,
                        },
                    },
                },
                {
                    id: "tableHead",
                    name: "Table Head",
                    basedOn: "Normal",
                    next: "Normal",
                    run: {
                        size: 14 * 2,
                        bold: true,
                        font: "黑体"
                    },
                    paragraph: {
                        alignment: docx.AlignmentType.CENTER
                    },
                },
                {
                    id: "listParagraph",
                    name: "List Paragraph",
                    basedOn: "Normal",
                    next: "Normal",
                    run: {
                        size: 14 * 2,
                        font: "宋体"
                    },
                    paragraph: {
                        spacing: {
                            line: 276,
                        },
                        indent: {
                            left: 112,
                        },
                    },
                },
            ],
        },
        numbering: {
            config: [
                {
                    reference: "listNumbering",
                    levels: [
                        {
                            level: 0,
                            format: docx.LevelFormat.DECIMAL,//"lowerLetter",
                            text: "%1.",
                            alignment: docx.AlignmentType.LEFT,
                        },
                        {
                            level: 1,
                            format: docx.LevelFormat.DECIMAL,//"lowerLetter",
                            text: "%1)",
                            alignment: docx.AlignmentType.LEFT,
                            style: {
                                paragraph: {
                                    indent: { left: 460 },
                                },
                            },
                        },
                    ],
                },
            ],
        },
        sections: [{
            children: [
                new docx.Paragraph({
                    text: title + "书",
                    heading: docx.HeadingLevel.TITLE,
                    alignment: docx.AlignmentType.CENTER,
                }),
                new docx.Paragraph({
                    style: "Heading1",
                    text: "基础数据",
                    heading: docx.HeadingLevel.HEADING_1,
                    numbering: {
                        reference: "listNumbering",
                        level: 0,
                    }
                }),
                table_form,
                new docx.Paragraph({
                    style: "Heading1",
                    text: "结果",
                    heading: docx.HeadingLevel.HEADING_1,
                    numbering: {
                        reference: "listNumbering",
                        level: 0,
                    }
                }),
                table_result,
                new docx.Paragraph({
                    children: [
                        new docx.TextRun({
                            text: "计算书由",

                        }),
                        new docx.ExternalHyperlink({
                            children: [
                                new docx.TextRun({
                                    text: window.location.hostname,
                                    style: "Hyperlink",
                                }),
                            ],
                            link: "https://" + window.location.hostname,
                        }),
                        new docx.TextRun("免费生成。"),
                    ],
                    spacing: {
                        before: 200,
                    },
                }),
            ],
        }],
    });
    docx.Packer.toBlob(doc).then(blob => {
        console.log(blob);
        saveAs(blob, title + "书.docx");
        console.log("Document created successfully");
    });
}