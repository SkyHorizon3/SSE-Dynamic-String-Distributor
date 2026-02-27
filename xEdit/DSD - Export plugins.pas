unit userscript;

var
    slData: TStringList;
    bFilled: bool;
    iMin: integer;
    sPluginName: string;

function Initialize: integer;
begin
    slData := TStringList.Create;
    slData.Add('[');
    bFilled := false;
    iMin := strtoint(InputBox('Minimum Local ID', 'If you want to skip ids, please enter the minimum FormID to export(i.e. oldest new one)', '0x0'));

    sPluginName := 'export.json'; // fallback
end;

function Process(e: IInterface): integer;
var
    i, i2, ix, iFormID, iLocalID, iEPFDCount, iNum: integer;
    s, sMasterPlugin, sFormID, sSignature, sEditorID: string;
    subRecord1, subRecord2, subRecord3, subRecord4: IInterface;
begin
    s := '';
    ix := -1;

    sPluginName := GetFileName(e);
    sMasterPlugin := GetFileName(MasterOrSelf(e));
    iFormID := FormID(MasterOrSelf(e));
    
    if (iFormID shr 24 = $FE) then
        iLocalID := iFormID and $FFF  // esl
    else
        iLocalID := iFormID and $FFFFFF; // normal
   
    if iLocalID < iMin then Exit;

    sFormID := IntToHex(iLocalID, 6);

    sEditorID := GetElementEditValues(e, 'EDID');
    sSignature := Signature(e);

    AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'FULL');
    AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'DESC');
    if sSignature = 'ACTI' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'RNAM');
    end else
    if sSignature = 'BOOK' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'CNAM');
    end else
    if sSignature = 'FLOR' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'RNAM');
    end else
    if sSignature = 'GMST' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'DATA\Name');
    end else
    if sSignature = 'INFO' then begin
        if ElementExists (e, 'Responses') then begin
            subRecord1 := ElementByName(e, 'Responses');
            for i:= 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                iNum := GetElementEditValues(subRecord2, 'TRDT\Response number');
                AddDataByPath(iNum, subRecord2, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'NAM1');
            end;
        end;
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'RNAM');
    end else
    if sSignature = 'MESG' then begin
        if ElementExists (e, 'Menu Buttons') then begin
            subRecord1 := ElementByName(e, 'Menu Buttons');
            for i := 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                AddDataByPath(i, subRecord2, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'ITXT');
            end;
        end;
    end else
    if sSignature = 'MGEF' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'DNAM');
    end else
    if sSignature = 'NPC_' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'SHRT');
    end else
    if sSignature = 'PERK' then begin
        if ElementExists (e, 'Effects') then begin
            subRecord1 := ElementByName(e, 'Effects');

            iEPFDCount := 0;

            for i:= 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                if ElementExists (subRecord2, 'Function Parameters') then begin
                    subRecord3 := ElementByName(subRecord2, 'Function Parameters');

                    if ElementExists(subRecord3, 'EPF2') then begin
                        ix := StrToInt(GetElementEditValues(subRecord3, 'EPF3\Fragment Index'));
                        AddDataByPath(ix, subRecord3, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'EPF2');
                    end;

                    if ElementExists(subRecord3, 'EPFD\Text') then begin
                        AddDataByPath(iEPFDCount, subRecord3, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'EPFD\Text');
                        iEPFDCount := iEPFDCount + 1;
                    end;

                end;
            end;
        end;
    end else
    if sSignature = 'QUST' then begin
        if ElementExists (e, 'Stages') then begin
            subRecord1 := ElementByName(e, 'Stages');
            for i:= 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                if ElementExists (subRecord2, 'Log Entries') then begin
                    subRecord3 := ElementByName(subRecord2, 'Log Entries');
                    iNum := GetElementEditValues(subRecord2, 'INDX\Stage Index');
                    
                    for i2:= 0 to ElementCount(subRecord3) - 1 do begin
                        subRecord4 := ElementByIndex(subRecord3, i2);
                        AddDataByPath(i2 + iNum, subRecord4, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'CNAM');
                    end;
                end;
            end;
        end;

        if ElementExists (e, 'Objectives') then begin
            subRecord1 := ElementByName(e, 'Objectives');
            for i:= 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                ix := StrToInt(GetElementEditValues(subRecord2, 'QOBJ'));
                AddDataByPath(ix, subRecord2, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'NNAM');
            end;
        end;
    end else
    if sSignature = 'REGN' then begin
        if ElementExists (e, 'Region Data Entries') then begin
            subRecord1 := ElementByName(e, 'Region Data Entries');
            for i:= 0 to ElementCount(subRecord1) - 1 do begin
                subRecord2 := ElementByIndex(subRecord1, i);
                AddDataByPath(i, subRecord2, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'RDMP');
            end;
        end;
    end else
    if sSignature = 'WOOP' then begin
        AddDataByPath(ix, e, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'TNAM');
    end else
    if sSignature = 'REFR' then begin
        if ElementExists (e, 'Map Marker') then begin
            subRecord1 := ElementByName(e, 'Map Marker');
            AddDataByPath(ix, subRecord1, s, sMasterPlugin, sFormID, sEditorID, sSignature, 'FULL');
        end;      
    end
end;

function EscapeJSONString(const s: string): string;
var
  i: Integer;
  ch: Char;
begin
  Result := '"';
  for i := 1 to Length(s) do begin
    ch := s[i];
    case Ord(ch) of
      34: Result := Result + '\"';  // "
      92: Result := Result + '\\';  // \
      47: Result := Result + '\/';  // /
      8:  Result := Result + '\b';
      9:  Result := Result + '\t';
      10: Result := Result + '\n';
      12: Result := Result + '\f';
      13: Result := Result + '\r';
    else
      if (Ord(ch) < 32) then // or (Ord(ch) > 127) then
        Result := Result + '\u' + IntToHex(Ord(ch), 4)
      else
        Result := Result + ch;
    end;
  end;
  Result := Result + '"';
end;

procedure AddDataByPath(indx: integer; e: IInterface; s, sMasterPlugin, sFormID, sEditorID, sSignature, path: string);
var
  sCleanPath, sFullType: string;
begin
    if not ElementExists(e, path) then Exit;
    s := GetElementEditValues(e, path);
    if (s <> '') then begin
        if bFilled then begin
            slData[slData.Count - 1] := slData[slData.Count - 1] + ',';
        end;

        bFilled := true;
        slData.Add('  {');
        slData.Add('    "form_id": "0x' + sFormID + '|' + sMasterPlugin + '",');
        if sEditorID <> '' then
            slData.Add('    "editor_id": "' + sEditorID + '",')
        else
            slData.Add('    "editor_id": null,');
       
        sCleanPath := path;
        if Pos('\', path) > 0 then
            sCleanPath := Copy(path, 1, Pos('\', path) - 1);

        sFullType := sSignature + ' ' + sCleanPath;

        slData.Add('    "type": "' + sFullType + '",');
        if (indx > -1) then
            slData.Add('    "index": ' + IntToStr(indx) + ',');

        slData.Add('    "string": ' + EscapeJSONString(s));
        slData.Add('  }');
    end;
end;

function Finalize: integer;
var
    dlgSave: TSaveDialog;
begin
    if bFilled then begin
        slData.Add(']');
        
        dlgSave := TSaveDialog.Create(nil);
        dlgSave.Options := dlgSave.Options + [ofOverwritePrompt];
        dlgSave.Filter := 'JSON (*.json)|*.json';
        dlgSave.InitialDir := DataPath;
        dlgSave.FileName := ChangeFileExt(sPluginName, '.json');
        if dlgSave.Execute then
            AddMessage('Saving ' + dlgSave.FileName);
            slData.SaveToFile(dlgSave.FileName);

        dlgSave.Free;
    end;
    slData.Free;
end;

end.