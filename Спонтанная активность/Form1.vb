'Программа демонстрирует принцип обучения коры возможности распространять идентификационные волны от паттернов вызванной активности
'
'Авторы:
'   Алексей Редозубов aldrd@yahoo.com
'   Дмитрий Шабанов shabanovd@gmail.com

'The MIT License (MIT)

'Copyright (c) 2014 Alexey Redozubov, Dmitriy Shabanov

'Permission is hereby granted, free of charge, to any person obtaining a copy
'of this software and associated documentation files (the "Software"), to deal
'in the Software without restriction, including without limitation the rights
'to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
'copies of the Software, and to permit persons to whom the Software is
'furnished to do so, subject to the following conditions:

'The above copyright notice and this permission notice shall be included in
'all copies or substantial portions of the Software.

'THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
'IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
'FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
'AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
'LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
'OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
'THE SOFTWARE.

Public Class Form1

    ' Dim TimeCheck(9) As double

    ' Размеры коры
    Dim X_C As Integer = 150
    Dim Y_C As Integer = 150


    ' Суммарная ритмическая активность
    Dim NSer As Integer = 300
    Dim CurPosA As Integer = 0
    Dim AWave(NSer - 1) As Double


    ' Параметры для формирования картины серии состояний
    Dim FPic As Boolean = False
    Dim PicStart As Integer
    Dim NPic As Integer = 16
    Dim WPic As Integer = 4

    Dim Bigpic As New Bitmap(X_C * WPic, Y_C * CInt(Math.Round(NPic / WPic) + 0.5))

    ' Текущий такт
    Dim CT As Integer = 0


    ' Вероятность истинно спонтанного спайка
    Dim Psp As Double = 0.02

    ' Кол-во типов нейронов
    Dim N_Types As Integer = 2


    ' Расстояние слежения за активностью для ID (влево и вправо по горизонтали и вертикали)
    Dim R_ID As Integer = 15
    ' Расстояние слежения за активностью для весов W (влево и вправо по горизонтали и вертикали)
    Dim R_W As Integer = R_ID

    ' Радиус распространения активности. Связан с расчетом активности на синапсах (трогать аккуратно)
    Dim RLAM As Integer = R_W

    ' шаблон распределения активности
    Dim NLocalAM = 2 * RLAM + 1
    Dim LocalAM(NLocalAM - 1, NLocalAM - 1)

    ' Параметр распространения активности. Дисперсия
    Dim Sigma2LAM As Single = 5 ^ 2

    ' Период полурапада распределенной активности (тактов)
    Dim TAhalf As Integer = 4
    'Падение активности за один такт
    Dim KA As Single = 1 / (2.0 ^ (1.0 / TAhalf))

    ' Площадь поля усреднения активности
    Dim SReceptF As Integer = (RLAM * 2 + 1) ^ 2

    ' Минимальная активность для начала обсчета
    Dim LAmin As Double = 0.008

    ' Предел активности для генерации спонтанных спайков
    Dim LAmax As Double = 0.025

    ' Предел количества нейронов в релаксирующем состоянии для каждого из типов. Выше этого предела подавляется спонтанная активность. 
    Dim LImin(N_Types - 1) As Double




    ' Порог активации по идентификатору
    Dim L_act As Double = 0.7

    ' количество волн, необходимых для консолидации
    Dim NCons As Integer = 5

    ' Продолжительность пакета идентификационной активности
    Dim T_ID As Integer = 10

    ' Продолжительность тишины
    Dim T_Passive As Integer = T_ID * 4



    Dim pic As New Bitmap(X_C, Y_C)
    Dim Gpic As Graphics



    ' Кол-во нейронов в патерне вызванной активности
    Dim NNPat As Integer = 15

    ' Радиус паттерна вызванной активности
    Dim R_Pat As Integer = 6

    ' Общее количество паттернов
    Dim NP As Integer = 5

    ' Количество активных паттернов в режиме волны
    Dim NPC As Integer = 1


    Structure Pattern
        ' Центры паттернов
        Dim ix As Integer
        Dim iy As Integer

        Dim P(,) As Byte

    End Structure


    ' Набор паттернов вызванной активности
    Dim PatSet(NP - 1) As Pattern



    Structure ID

        ' 0 - не консолидирован
        ' 1 - консолидирован
        Dim ConsStatus As Integer

        Dim NAct As Integer
        Dim TCreat As Integer

        Dim NP As Integer ' Номер паттерна для проверки во время отладки

        Dim P(,) As Byte

    End Structure

   
    Structure Neuron

        ' Тип нейрона
        Dim Type As Byte
        ' 0 - входной аксон
        ' 1 - нейрон первого уровня (волновой)
        ' 2 - нейрон второго уровня

        ' Текущий статус нейрона
        Dim Status As Byte
        ' 0 - спокоен
        ' 1 - вызванная активность
        ' 2 - волновая активность
        ' 3 - первый такт волновой активности
        ' 4 - релаксация после волнового спайка
        ' 5 - спонтанная активность
        ' 6 - входной аксон
        ' 7 - состояние проверки на возможное участие в волновом идентификаторе

        ' Накопленная за время обучения синапсов активность
        Dim AAccumSinapse As Single

        ' Накопленная за время обучения внесинаптических рецепторов активность
        Dim AAccumExtraSinapse As Single

        ' Временное событие
        Dim T As Integer

        ' Вызванный потенциал активности нерона в мощностном измерении
        Dim EvokedA As Single


        ' Текущий статус обучения нейрона
        Dim StatusL As Byte
        ' 0 - свободен
        ' 1 - фасилитация
        ' 2 - обучен
        ' 5 - не подлежит обучению (входной аксон)

        ' Временное событие обучения
        Dim TL As Integer


        ' Набор картин идентификаторв индивидуальных для каждого нейрона
        Dim ID As Collection

        Dim ERR As Boolean

    End Structure


    ' Кора
    Dim Cortex(X_C - 1, Y_C - 1) As Neuron

    ' Число тактов накопления активности при синаптическом обучении
    Dim T_SinapseLearn As Integer = T_Passive

    ' Число тактов накопления активности при внесинаптическом обучении
    Dim T_ExtraSinapseLearn As Integer = 4 ' T_ID


    ' Набор полей последних состояний нейронов. 
    Dim CortexA(X_C - 1, Y_C - 1, T_SinapseLearn - 1) As Byte

    ' Текущая последовательность слоев
    Dim LayersSequence(T_SinapseLearn - 1) As Integer

    ' Текущий слой
    Dim CurLayer As Integer = 0


    ' Порог активации нейрона
    Dim L_Spike As Single = 0.6

    ' Продолжительность вызванной активности
    Dim T_EnvokedA As Integer = T_Passive * 1.5


    ' Синапсы 

    Dim N_Neurotransmitters As Integer = 2

    ' Таблица чувствительности синаптических и внесинаптических рецепторов к медиаторам, генерируемым аксонами нейронов разного типа

    Dim SensTable(N_Types - 1, N_Types - 1, 0 To 1) As Byte


    ' Веса синапсов нейронов
    Dim CortexW(X_C - 1, Y_C - 1, R_W * 2, R_W * 2) As Single



    ' Вторичная зона коры
    Dim cortex2 As Form1 = Nothing

    ' Размеры туннеля
    Dim TD As Integer = 30

    ' Координаты угла исходящего туннеля
    Dim TX1 As Integer = X_C / 4
    Dim TY1 As Integer = Y_C / 4

    ' Координаты угла входящего туннеля
    Dim TX2 As Integer = X_C / 2
    Dim TY2 As Integer = Y_C / 2



    ' Флаг включения обучения нейронов-детекторов
    Dim FLearn As Boolean = False



    ' Время, отведенное на консолидацию
    Dim T_L1 As Integer = 50


    ' Поле накопленной вызванной активности коры
    Dim Field_A_Accum(X_C - 1, Y_C - 1) As Single

    ' Поле мгновенной активности коры 
    Dim Field_A(X_C - 1, Y_C - 1) As Single

    ' Поле вызванной активности коры
    Dim Field_A_Evoced(X_C - 1, Y_C - 1) As Single

    ' Поле волновой активности коры
    Dim Field_A_Wave(X_C - 1, Y_C - 1) As Single

    ' Поле плотности релаксирующих нейронов каждого из типов
    Dim Field_A_Relax(X_C - 1, Y_C - 1, N_Types - 1) As Single






    ' Порог окружающей вызванной активности для возможности обучения
    Dim L_ActL1 As Single = 0.15

    ' Порог активности на синапсах для возможности обучения
    Dim L_ActLS As Single = (2 * R_W + 1) ^ 2 * 0.02





    ' Коэффициенты пространственной самоорганизации
    ' Порог умирания от одиночества
    Dim L_GameLive_Low = 0.15
    ' Порог умирания от перенасыщения
    Dim L_GameLive_high = 0.25






    ' Инициализация
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        SetSens()

        PatCreate()

        CheckedListBox1.CheckOnClick = True




        ' Задание пороговых уровней плотности релаксирующих нейронов для подавления спонтанной активности
        LImin(0) = 0.01
        LImin(1) = 0.01


        ComboBox1.SelectedIndex = 0
        ComboBox2.SelectedIndex = 0


        ' Инициация нейронов коры
        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1


                With Cortex(ix, iy)

                    .Type = 0

                    .ID = New Collection

                End With

            Next
        Next

        Gpic = Graphics.FromImage(pic)

        ' Создание шаблона распрстранения активности
        ' Сумма всех точек дает единицу. Это позволяет устанавливать пороги для полей в процентах задействованных нейронов
        Dim dx, dy As Single

        For ix = 0 To NLocalAM - 1
            For iy = 0 To NLocalAM - 1

                dx = ix - RLAM
                dy = iy - RLAM


                LocalAM(ix, iy) = (1 / (2 * Math.PI * Sigma2LAM)) * Math.Exp(-1 / (2 * Sigma2LAM) * (dx ^ 2 + dy ^ 2))

            Next
        Next

    End Sub

    ' Задание таблицы чувтвительности рецепторов
    Private Sub SetSens()

        ' Волновые нейроны (тип 0):
        ' Медиатор аксона - A
        ' внесинаптические рецепторы - A
        ' синаптические рецепторы - B

        ' Нейроны детекторы (тип 1):
        ' Медиатор аксона - A, B
        ' внесинаптические рецепторы - ---
        ' синаптические рецепторы - A

        ' Формат таблицы
        ' тип передающего нейрона, тип принимающего, чувствительность внесинаптическая (0) - синаптическая (1)

        SensTable(0, 0, 0) = 1
        SensTable(0, 0, 1) = 0

        SensTable(0, 1, 0) = 0
        SensTable(0, 1, 1) = 1

        SensTable(1, 0, 0) = 1
        SensTable(1, 0, 1) = 1

        SensTable(1, 1, 0) = 0
        SensTable(1, 1, 1) = 0


    End Sub
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        PatCreate()
    End Sub

    ' Создание случайных паттернов вызванной активности
    Private Sub PatCreate()

        Dim dx, dy As Integer

        CheckedListBox1.Items.Clear()

        For i = 0 To X_C - 1
            For j = 0 To Y_C - 1

                pic.SetPixel(i, j, Color.Black)

            Next
        Next

        For i = 0 To NP - 1


            With PatSet(i)
                .ix = (X_C - 3 * R_Pat) * Rnd() + R_Pat
                .iy = (Y_C - 3 * R_Pat) * Rnd() + R_Pat


                Dim Ang As Double
                Dim R As Double

                ReDim .P(R_Pat * 2, R_Pat * 2)

                For j = 0 To NNPat - 1

                    Ang = 2 * Math.PI * Rnd()
                    R = R_Pat * Rnd()

                    dx = R * Math.Sin(Ang)

                    dy = R * Math.Cos(Ang)

                    .P(R_Pat + dx, R_Pat + dy) = 1

                    ' Присвоение статуса входных волокон
                    Cortex(.ix + dx, .iy + dy).StatusL = 5

                Next


                For ix = 0 To 2 * R_Pat

                    For iy = 0 To 2 * R_Pat

                        If .P(ix, iy) = 1 Then
                            pic.SetPixel(.ix - R_Pat + ix, .iy - R_Pat + iy, Color.White)
                        End If

                    Next
                Next


            End With

            CheckedListBox1.Items.Add(i)


        Next

        PictureBox1.Image = pic

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        Wave()
    End Sub

    ' 50 тактов волны
    Private Sub Button11_Click(sender As Object, e As EventArgs) Handles Button11.Click

        Dim NT As Integer = 50

        For n = 0 To NT - 1
            Wave()
        Next

    End Sub

    ' 200 тактов волны
    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        Dim NT As Integer = 200

        For n = 0 To NT - 1
            Wave()
        Next

    End Sub

    ' Такт волны
    Public Sub Wave()


        Dim AW As Double

        Label1.Text = CT.ToString
        Label1.Update()


        ' Расчет полей состояния коры
        CalcFields()


        ' Отображение выбранного поля

        Select Case ComboBox1.SelectedIndex

            Case 0
                Draw_ActAccum()
            Case 1
                Draw_FieldA()
            Case 2
                Draw_EvokedA()
            Case 3
                Draw_WaveA()
            Case 4
                Draw_RelaxA()
        End Select


        ' Определяем переходы состояний нейронов

        For ix = R_ID To X_C - R_ID - 1
            For iy = R_ID To Y_C - R_ID - 1

                With Cortex(ix, iy)

                    Select Case .Status

                        Case 0 ' покой

                            ' определяем узнавание идентификатора, но не корректируем CortexA, а только меняем статус

                            S0(ix, iy)

                            If .Status = 0 Then
                                ' Если возбуждения нет, уровень окружающей активности высок, но еще не достиг верхнего порога, 
                                ' а блокированных нейронов вокруг мало, то
                                ' создаем кандидата на добавление в идентификатор

                                If Field_A(ix, iy) > LAmin And Field_A_Wave(ix, iy) < LAmax And Field_A_Relax(ix, iy, Cortex(ix, iy).Type) < LImin(Cortex(ix, iy).Type) Then

                                    If Rnd() < Psp Then

                                        .Status = 7
                                        .T = CT + T_ExtraSinapseLearn

                                    End If

                                End If

                            End If

                        Case 7

                            S0(ix, iy)

                            If .Status = 7 Then

                                If Field_A_Wave(ix, iy) > LAmax Then
                                    ' Сбрасываем состояние кандидата, если активность превышает порог
                                    .Status = 0


                                Else

                                    If CT > .T Then
                                        ' вызываем спонтанную активность и проводим ID обучение 

                                        Cortex(ix, iy).Status = 5
                                        Cortex(ix, iy).T = CT

                                        S1(ix, iy)

                                    End If

                                End If

                            End If

                        Case 1 ' вызванная активность


                            ' Сбрасываем вызванную активность по окончании пакета
                            If CT > .T Then

                                .Status = 0

                            End If


                        Case 2 ' пачка волновой активности

                            ' если пачка закончилась, то переводим в состояние нечувствительности
                            If CT > .T Then


                                ' задаем время релаксации тем больше, чем сильнее рядом вызванная активность
                                .T = CT + T_Passive ' * (1 + Field_A_Accum(ix, iy) / 0.02)
                                .Status = 4

                                .ERR = False

                            End If

                        Case 3 ' первый такт волновой активности

                            .Status = 2

                        Case 4 ' состояние нечувствительности

                            ' 
                            If CT > .T Then

                                .Status = 0


                            End If

                        Case 5 ' спонтанный спайк

                            .Status = 2

                        Case 6 ' Проекционная (входная) активность



                    End Select

                End With

            Next
        Next

        ' Создаем картину активности нового временного слоя, соответсвующую статусам

        AW = 0

        ' Сдвигаем текщий временной слой
        CurLayer += 1
        If CurLayer = T_SinapseLearn Then CurLayer = 0

        For ix = R_ID To X_C - R_ID - 1
            For iy = R_ID To Y_C - R_ID - 1

                Select Case Cortex(ix, iy).Status

                    Case 1, 2, 3, 5, 6

                        CortexA(ix, iy, CurLayer) = 1

                        AW += 1

                    Case Else

                        CortexA(ix, iy, CurLayer) = 0

                End Select

            Next
        Next



        ' График ритма
        CurPosA += 1
        If CurPosA = NSer Then CurPosA = 0
        AWave(CurPosA) = AW / ((X_C - 2 * R_ID) * (Y_C - 2 * R_ID))
        DrawChart()

        ' Проверяем на консолидацию или удаление идентификационных следов 

        ' ---------------------------- 

        Draw_Status()


        CT += 1

        If FPic Then

            addpic()

        End If

        ' Установка последовательности слоев (0 - текущий, 1 - предыдущий и т.д.)

        Dim k As Integer = CurLayer

        For i = 0 To T_SinapseLearn - 1

            LayersSequence(i) = k
            k -= 1
            If k < 0 Then k = T_SinapseLearn - 1

        Next


        ' Расчет накопленной по времени активности
        Dim sum As Integer

        For ix = R_ID To X_C - R_ID - 1
            For iy = R_ID To Y_C - R_ID - 1

                sum = 0

                For i = 0 To T_ExtraSinapseLearn - 1

                    sum += CortexA(ix, iy, LayersSequence(i))

                Next

                If sum > 0 Then
                    Cortex(ix, iy).AAccumExtraSinapse = 1
                Else
                    Cortex(ix, iy).AAccumExtraSinapse = 0
                End If


                For i = T_ExtraSinapseLearn To T_SinapseLearn - 1

                    sum += CortexA(ix, iy, LayersSequence(i))

                Next

                If sum > 0 Then
                    Cortex(ix, iy).AAccumSinapse = 1
                Else
                    Cortex(ix, iy).AAccumSinapse = 0
                End If


            Next
        Next




        ' Обучение
        If FLearn Then


            ' Расчет вызванной активности
            Calc_Evoked_Activity()


            'MakePicFieldA()


            ' Обучение синаптических весов

            ' Задание фасилитации
            LearnW1()

            ' Отмена фасилитации, исходя из условий выживания
            'LearnW2()

            Select Case ComboBox2.SelectedIndex

                Case 0
                    Draw_Learn()
                Case 1

                Case 2

                Case 3

                Case 4

            End Select



        End If



        ' Проекция по волновому туннелю
        TMove()

    End Sub

    ' Расчет вызванной активности
    Private Sub Calc_Evoked_Activity()

        Dim x1, x2, sum, n1, n2 As Single

        For ix = R_W To X_C - R_W - 1
            For iy = R_W To Y_C - R_W - 1

                With Cortex(ix, iy)

                    .EvokedA = 0

                    If .StatusL > 0 Then

                        sum = 0
                        n1 = 0
                        n2 = 0

                        For ix1 = 0 To 2 * R_W
                            For iy1 = 0 To 2 * R_W

                                x1 = Cortex(ix + ix1 - R_W, iy + iy1 - R_W).AAccumSinapse
                                x2 = CortexW(ix, iy, ix1, iy1)

                                sum += x1 * x2

                                n1 += x1
                                n2 += x2

                            Next
                        Next

                        If sum > 0 Then

                            .EvokedA = sum / Math.Sqrt(n1 * n2)

                        End If


                        If .EvokedA > L_Spike Then

                            If .Status <> 1 Then
                                .T = CT + T_EnvokedA
                            End If

                            .Status = 1


                        End If

                    End If

                End With

            Next
        Next


    End Sub



    ' Рассчет полей состояния коры
    Private Sub CalcFields()

        Dim x, y, T As Integer
        Dim s As Single

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1

                Field_A_Accum(ix, iy) = Field_A_Accum(ix, iy) * KA
                Field_A(ix, iy) = 0
                Field_A_Wave(ix, iy) = 0
                Field_A_Evoced(ix, iy) = 0

                For i = 0 To N_Types - 1

                    Field_A_Relax(ix, iy, i) = 0

                Next

            Next
        Next


        For ix = RLAM To X_C - RLAM - 1
            For iy = RLAM To Y_C - RLAM - 1



                Select Case Cortex(ix, iy).Status

                    Case 1, 6

                        For ix1 = 0 To NLocalAM - 1
                            For iy1 = 0 To NLocalAM - 1

                                x = ix + ix1 - RLAM
                                y = iy + iy1 - RLAM

                                s = LocalAM(ix1, iy1)

                                Field_A_Accum(x, y) += s

                                Field_A(x, y) += s

                                Field_A_Evoced(x, y) += s

                            Next
                        Next

                    Case 2, 3, 5

                        For ix1 = 0 To NLocalAM - 1
                            For iy1 = 0 To NLocalAM - 1

                                x = ix + ix1 - RLAM
                                y = iy + iy1 - RLAM

                                s = LocalAM(ix1, iy1)

                                Field_A(x, y) += s

                                Field_A_Wave(x, y) += s

                            Next
                        Next

                    Case 4

                        T = Cortex(ix, iy).Type

                        For ix1 = 0 To NLocalAM - 1
                            For iy1 = 0 To NLocalAM - 1

                                x = ix + ix1 - RLAM
                                y = iy + iy1 - RLAM

                                s = LocalAM(ix1, iy1)

                                Field_A_Relax(x, y, T) += s

                            Next
                        Next


                End Select

            Next
        Next


    End Sub



    ' Обучение синаптических весов
    ' Задание фасилитации
    Private Sub LearnW1()


        For ix = R_W To X_C - R_W - 1
            For iy = R_W To Y_C - R_W - 1

                If Cortex(ix, iy).Status = 2 And Field_A_Accum(ix, iy) > L_ActL1 And Field_A(ix, iy) > L_ActLS And Cortex(ix, iy).StatusL = 0 Then


                    Cortex(ix, iy).StatusL = 1

                    Cortex(ix, iy).TL = CT


                    ' Фиксация кандидата в прототип
                    For ix1 = 0 To 2 * R_W
                        For iy1 = 0 To 2 * R_W

                            CortexW(ix, iy, ix1, iy1) = Cortex(ix + ix1 - R_W, iy + iy1 - R_W).AAccumSinapse '* SensTable(Cortex(ix + ix1 - R_W, iy + iy1 - R_W).Type, Cortex(ix, iy).Type, 1)

                        Next
                    Next

                End If

            Next
        Next

    End Sub

   


    ' S0
    ' Обработка нейрона в состоянии покоя
    Private Sub S0(ByRef ix As Integer, ByRef iy As Integer)



        ' Выходим если уровень активности вокруг нейрона мал и не считаем все возможные слои
        If Field_A(ix, iy) < LAmin Then Exit Sub

        ' Если уровень окружающей активности высок, то проверяем на необходимость волнового возбуждения

        If Cortex(ix, iy).ID.Count > 0 Then

            Dim sum, NN, NP, P As Integer

            For Each S As ID In Cortex(ix, iy).ID

                With S

                    sum = 0
                    NN = 0

                    NP = 0


                    For ix1 = ix - R_ID To ix + R_ID
                        For iy1 = iy - R_ID To iy + R_ID

                            P = .P(ix1 - ix + R_ID, iy1 - iy + R_ID)
                            ' Учитывается соответсвие типов медиаторов и рецепторов
                            sum += Cortex(ix1, iy1).AAccumExtraSinapse * P * SensTable(Cortex(ix1 - ix + R_ID, iy1 - iy + R_ID).Type, Cortex(ix, iy).Type, 0)

                            NN += P

                        Next
                    Next


                    If sum / CDbl(NN) > L_act Then

                        ' Если наступает волновое возбуждение разбираемся с консолидацией МРК

                        If .ConsStatus = 0 Then

                            .NAct += 1
                            If .NAct > NCons Then .ConsStatus = 1

                        End If


                        Cortex(ix, iy).Status = 3

                        Cortex(ix, iy).T = CT + T_ID

                        ' для ускорения можно выйти из цикла, но это может нарушить консолидацию остальных слоев 

                        If CheckedListBox1.CheckedItems.Count = 1 And CheckedListBox1.CheckedItems.Item(0) <> .NP Then Cortex(ix, iy).ERR = True


                    End If

                End With

            Next

        End If

    End Sub

    ' S1
    ' Создание метаботропного рецептивного кластера
    Private Sub S1(ByRef ix As Integer, ByRef iy As Integer)

        ' Создаем образ окружающего паттерна (метаботропный рецептивный кластер)

        Dim S As New ID

        With S

            .ConsStatus = 0
            .NAct = 1
            .TCreat = CT

            ReDim .P(R_ID * 2, R_ID * 2)


            For ix1 = ix - R_ID To ix + R_ID
                For iy1 = iy - R_ID To iy + R_ID

                    ' учитывем чувствительность к медиатору
                    .P(ix1 - ix + R_ID, iy1 - iy + R_ID) = Cortex(ix1, iy1).AAccumExtraSinapse * SensTable(Cortex(ix1 - ix + R_ID, iy1 - iy + R_ID).Type, Cortex(ix, iy).Type, 0)

                Next
            Next

            ' Заполняем к какому паттерну относится этот МК (для контроля)
            If CheckedListBox1.CheckedItems.Count = 1 Then

                .NP = CheckedListBox1.CheckedItems.Item(0)

            Else

                .NP = 0

            End If
            

        End With

        Cortex(ix, iy).ID.Add(S)


    End Sub



    ' Картина общего состояния коры (статус нейронов)
    Private Sub Draw_Status()

        For i = 0 To X_C - 1
            For j = 0 To Y_C - 1


                Select Case Cortex(i, j).Status


                    Case 0

                        pic.SetPixel(i, j, Color.Black)


                    Case 1 ' Вызванная активность
                        pic.SetPixel(i, j, Color.Orange)

                    Case 2 ' Спайки волны идентификатора

                        pic.SetPixel(i, j, Color.Gray)

                        If Cortex(i, j).ERR Then pic.SetPixel(i, j, Color.Red)

                    Case 3 ' Фронт волны идентификатора

                        pic.SetPixel(i, j, Color.White)

                        If Cortex(i, j).ERR Then pic.SetPixel(i, j, Color.Red)


                    Case 4 ' Нейроны в заторможенном состоянии

                        pic.SetPixel(i, j, Color.DarkBlue)


                    Case 5 ' Истинно спонтанный спайк

                        pic.SetPixel(i, j, Color.Green)

                    Case 6 ' Проекционная активность
                        pic.SetPixel(i, j, Color.Red)


                End Select

            Next
        Next


        ' Если есть волновой туннель, то рисование его границ
        If (Me.Text = "Form1" And Not (cortex2 Is Nothing)) Or Me.Text = "2" Then

            Dim Pen As New Pen(Color.White, 1)

            Dim i1, i2 As Integer


            If Not (cortex2 Is Nothing) Then

                i1 = TX1
                i2 = TY1
            Else
                i1 = TX2
                i2 = TY2

            End If


            Gpic.DrawLine(Pen, i1, i2, i1 + TD, i2)
            Gpic.DrawLine(Pen, i1, i2 + TD, i1 + TD, i2 + TD)
            Gpic.DrawLine(Pen, i1, i2, i1, i2 + TD)
            Gpic.DrawLine(Pen, i1 + TD, i2, i1 + TD, i2 + TD)

        End If

        PictureBox1.Image = pic
        PictureBox1.Update()


    End Sub







    ' Картина вызванной активности
    Private Sub Draw_EvokedA()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1


                Y = Field_A_Evoced(ix, iy) / 0.08 * 255

                If Y > 255 Then Y = 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))

                
            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub


    ' Картина накопленной активности
    Private Sub Draw_ActAccum()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1

                Y = Field_A_Accum(ix, iy) * 500

                If Y > 255 Then Y = 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))

            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub


    ' Картина поля активности (размытая)
    Private Sub Draw_FieldA()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1


                Y = Field_A(ix, iy) / 0.08 * 255

                If Y > 255 Then Y = 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))


            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub

    ' Картина релаксации
    Private Sub Draw_RelaxA()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1


                Y = Field_A_Relax(ix, iy, 0) / 0.04 * 255

                If Y > 255 Then Y = 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))


            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub

    ' Картина поля волны (размытая)
    Private Sub Draw_WaveA()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1


                Y = Field_A_Wave(ix, iy) / 0.04 * 255

                If Y > 255 Then Y = 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))


            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub

    ' Картина поля активности, усредненного по времении
    Private Sub MakePicAAvg()

        Dim pic1 As New Bitmap(X_C, Y_C)

        Dim Y As Integer

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1

                Y = Cortex(ix, iy).AAccumSinapse * 255

                pic1.SetPixel(ix, iy, Color.FromArgb(Y, Y, Y))

            Next
        Next

        PictureBox2.Image = pic1
        PictureBox2.Update()

    End Sub




    ' Картина состояния обучения
    Private Sub Draw_Learn()

        Dim pic1 As New Bitmap(X_C, Y_C)

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1

                Select Case Cortex(ix, iy).StatusL

                    Case 0

                        pic1.SetPixel(ix, iy, Color.Black)

                    Case 1

                        If Cortex(ix, iy).Status = 1 Then

                            If Field_A_Accum(ix, iy) < L_GameLive_Low Then
                                pic1.SetPixel(ix, iy, Color.Blue)
                            End If
                            If Field_A_Accum(ix, iy) >= L_GameLive_Low And Field_A_Accum(ix, iy) < L_GameLive_high Then
                                pic1.SetPixel(ix, iy, Color.Green)
                            End If
                            If Field_A_Accum(ix, iy) >= L_GameLive_high Then
                                pic1.SetPixel(ix, iy, Color.Red)
                            End If


                        Else

                            pic1.SetPixel(ix, iy, Color.Yellow)

                        End If

                    Case 2

                        If Cortex(ix, iy).Status = 1 Then

                            pic1.SetPixel(ix, iy, Color.White)
                        Else

                            pic1.SetPixel(ix, iy, Color.DarkBlue)

                        End If
                    Case 5
                        pic1.SetPixel(ix, iy, Color.DarkMagenta)

                End Select

            Next
        Next

        PictureBox4.Image = pic1
        PictureBox4.Update()

    End Sub



    Private Sub CheckedListBox1_MouseUp(sender As Object, e As MouseEventArgs) Handles CheckedListBox1.MouseUp
        PatternsActivation()
    End Sub
    ' Активация входных паттернов
    Private Sub PatternsActivation()
        For i = 0 To X_C - 1
            For j = 0 To Y_C - 1

                Cortex(i, j).Status = 0
                CortexA(i, j, CurLayer) = 0

            Next
        Next

        For Each item In CheckedListBox1.CheckedItems

            With PatSet(item)

                For ix = 0 To 2 * R_Pat

                    For iy = 0 To 2 * R_Pat

                        If .P(ix, iy) = 1 Then
                            Cortex(.ix - R_Pat + ix, .iy - R_Pat + iy).Status = 6
                        End If

                    Next
                Next
            End With
        Next

        Draw_Status()

        PictureBox1.Image = pic
    End Sub

    ' График суммарного ритма
    Private Sub DrawChart()


        Dim YSize = 40
        Dim Y As Integer

        Dim Amax = 0.05

        Dim pic2 As New Bitmap(NSer, YSize)

        Dim i As Integer = 0


        For k = CurPosA + 1 To NSer - 1

            Y = AWave(k) * (YSize - 1) / Amax
            If Y >= YSize - 1 Then Y = YSize - 1
            pic2.SetPixel(i, YSize - Y - 1, Color.Black)

            i += 1

        Next


        For k = 0 To CurPosA

            Y = AWave(k) * (YSize - 1) / Amax
            If Y >= YSize - 1 Then Y = YSize - 1
            pic2.SetPixel(i, YSize - Y - 1, Color.Black)

            i += 1

        Next

        PictureBox3.Image = pic2
        PictureBox3.Update()

    End Sub


    ' вкл серии картинок
    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        FPic = Not FPic

        If FPic Then
            Label2.Text = "вкл."
            PicStart = CT
        Else
            Label2.Text = "выкл."
        End If

    End Sub


    ' Формирование серии картинок
    Private Sub addpic()

        Dim s As Integer = CT - PicStart - 1

        Dim Y As Integer = Int(s / WPic)
        Dim X As Integer = s - Y * WPic

        X = X * X_C
        Y = Y * Y_C

        For ix = 0 To X_C - 1
            For iy = 0 To Y_C - 1

                Bigpic.SetPixel(X + ix, Y + iy, pic.GetPixel(ix, iy))

            Next
        Next

        If s = NPic - 1 Then
            FPic = False
            Label2.Text = "выкл."
            Label2.Update()

            PictureBox1.Image = Bigpic
            PictureBox1.Update()

            Me.SetTopLevel(True)
            MsgBox("")


        End If

    End Sub

    ' Проекция по волновому туннелю
    Public Sub TMove()

        If cortex2 Is Nothing Then Exit Sub

        For ix = 0 To TD - 1
            For iy = 0 To TD - 1

                cortex2.Cortex(ix + TX2, iy + TY2).Status = Cortex(ix + TX1, iy + TY1).Status
                cortex2.CortexA(ix + TX2, iy + TY2, cortex2.CurLayer) = CortexA(ix + TX1, iy + TY1, CurLayer)
                cortex2.Cortex(ix + TX2, iy + TY2).T = Cortex(ix + TX1, iy + TY1).T

            Next
        Next

        cortex2.Wave()

    End Sub

    ' Создание волнового туннеля
    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click

        cortex2 = New Form1

        cortex2.Text = "2"
        cortex2.Show()



    End Sub


    ' Вкл/выкл обучения
    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        FLearn = Not FLearn

        If FLearn Then
            Label3.Text = "вкл."
        Else
            Label3.Text = "выкл."
        End If

    End Sub



   
End Class

